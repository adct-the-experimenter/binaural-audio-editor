#include "mono-audio-track.h"

MonoAudioTrack::MonoAudioTrack(const wxString& title) : Track(title)
{
	sourceToManipulatePtr = nullptr;
	audioPlayerPtr = nullptr;
	
	track_state = State::PLAYER_NULL;
	
	//initialize audio tracks
	m_channel_track = new AudioTrack("Channel");
	
	Connect(wxEVT_PAINT, wxPaintEventHandler(MonoAudioTrack::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(MonoAudioTrack::OnSize));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MonoAudioTrack::OnLeftMouseClick));
	Connect(wxEVT_CONTEXT_MENU, wxCommandEventHandler(MonoAudioTrack::OnRightMouseClick));
	
	inputSoundFilePath = "";
	streamSoundFilePath = "../resources/stream.wav";
	
	
	playbackControlsPtr = nullptr;
	
}

void MonoAudioTrack::SetReferenceToSourceToManipulate(ALuint* source){sourceToManipulatePtr = source;}
void MonoAudioTrack::SetReferenceToAudioPlayer(OpenALSoftPlayer* thisPlayer){audioPlayerPtr = thisPlayer;}

void MonoAudioTrack::SetReferenceToBrowseButton(wxButton* thisButton){browseButton = thisButton;}

AudioTrack* MonoAudioTrack::GetReferenceToChannelTrack(){return m_channel_track;}

//Track related functions
void MonoAudioTrack::al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = nsec / 1000000000ul;
    ts.tv_nsec = nsec % 1000000000ul;
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR){ts = rem;}
}

void MonoAudioTrack::FunctionToCallInPlayState()
{
	if(sourceToManipulatePtr != nullptr)
	{
		double current_time = m_channel_track->GetCurrentTime();
	
		//get current state and do things differently depending on current state
		switch(MonoAudioTrack::GetAudioTrackState())
		{
			case State::PLAYER_NULL:
			{
				audioPlayerPtr->StartPlayer(sourceToManipulatePtr,current_time); //start player
				
				break;
			}
			case State::PLAYER_PAUSED:
			{
				//audioPlayerPtr->PlaySource(sourceToManipulatePtr);
				//audioPlayerPtr->UpdatePlayer(sourceToManipulatePtr,current_time);
				audioPlayerPtr->ClearQueue(sourceToManipulatePtr);
				audioPlayerPtr->StartPlayer(sourceToManipulatePtr,current_time);
				MonoAudioTrack::al_nssleep(10000000);
				break;
			}
			case State::PLAYER_PLAYING:
			{
				switch(audioPlayerPtr->UpdatePlayer(sourceToManipulatePtr,current_time))
				{
					case OpenALSoftPlayer::PlayerStatus::PLAYBACK_FINISHED:
					{
						std::cout << "Playback finished! \n";
						
						playbackControlsPtr->StopOP();
						break;
					}
					case OpenALSoftPlayer::PlayerStatus::FAILED_TO_READ_ANYMORE_AUDIO_FROM_FILE:
					{
						std::cout << "No more audio to read! \n";
						
						playbackControlsPtr->StopOP();
						break;
					}
					
				}
				
				MonoAudioTrack::al_nssleep(10000000);
				break;
			}
			case State::PLAYER_REWINDING:
			{
				audioPlayerPtr->StartPlayer(sourceToManipulatePtr,current_time);
				MonoAudioTrack::al_nssleep(10000000);
				break;
			}
			case State::PLAYER_FAST_FORWARDING:
			{
				audioPlayerPtr->StartPlayer(sourceToManipulatePtr,current_time);
				MonoAudioTrack::al_nssleep(10000000);
				break;
			}
			
		}		
	}
	
	MonoAudioTrack::SetAudioTrackState(PLAYER_PLAYING); //switch to player playing state
	
}

void MonoAudioTrack::FunctionToCallInPauseState()
{
	if(MonoAudioTrack::GetAudioTrackState() != State::PLAYER_PAUSED)
	{
		MonoAudioTrack::SetAudioTrackState(State::PLAYER_PAUSED);
	
		if(sourceToManipulatePtr != nullptr)
		{
			//audioPlayerPtr->PauseSource(sourceToManipulatePtr);
			audioPlayerPtr->RewindSource(sourceToManipulatePtr);
			audioPlayerPtr->ClearQueue(sourceToManipulatePtr);
		}
		
	}
	
}

void MonoAudioTrack::FunctionToCallInRewindState()
{
	//clear buffers in queue and rewind source
	if(MonoAudioTrack::GetAudioTrackState() != State::PLAYER_REWINDING)
	{
		if(sourceToManipulatePtr != nullptr)
		{
			audioPlayerPtr->RewindSource(sourceToManipulatePtr);
			audioPlayerPtr->ClearQueue(sourceToManipulatePtr);
		}
	}
	MonoAudioTrack::SetAudioTrackState(State::PLAYER_REWINDING);

}

void MonoAudioTrack::FunctionToCallInFastForwardState()
{
	//clear buffers in queue and rewind source
	if(MonoAudioTrack::GetAudioTrackState() != State::PLAYER_FAST_FORWARDING)
	{
		if(sourceToManipulatePtr != nullptr)
		{
			audioPlayerPtr->RewindSource(sourceToManipulatePtr);
			audioPlayerPtr->ClearQueue(sourceToManipulatePtr);
		}
	}
	MonoAudioTrack::SetAudioTrackState(State::PLAYER_FAST_FORWARDING);
}

void MonoAudioTrack::FunctionToCallInNullState()
{
	if(MonoAudioTrack::GetAudioTrackState() != State::PLAYER_NULL)
	{
		MonoAudioTrack::SetAudioTrackState(MonoAudioTrack::State::PLAYER_NULL);
		if(sourceToManipulatePtr != nullptr)
		{
			audioPlayerPtr->StopSource(sourceToManipulatePtr);
			audioPlayerPtr->RewindSource(sourceToManipulatePtr);
			audioPlayerPtr->ClearQueue(sourceToManipulatePtr);
		}
	}
}

void MonoAudioTrack::SetAudioTrackState(int thisState)
{
	track_state = thisState;
	m_channel_track->SetAudioTrackState(thisState);
}

int MonoAudioTrack::GetAudioTrackState(){return track_state;}
    
void MonoAudioTrack::SetReferenceToCurrentTimeVariable(double* thisTimeVariable){Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);}
void MonoAudioTrack::SetReferenceToTimeTickVector(std::vector <int> *thisVector){Track::SetReferenceToTimeTickVector(thisVector);}

std::vector <int> *MonoAudioTrack::GetReferenceToTimeTickVector(){return Track::GetReferenceToTimeTickVector();}

void MonoAudioTrack::SetTitle(wxString thisTitle){Track::SetTitle(thisTitle);}
wxString MonoAudioTrack::GetTitle(){return Track::GetTitle();}

void MonoAudioTrack::InitTrack(wxWindow* parent, std::vector <int> *timeTickVector)
{
	browseButton->Bind(wxEVT_BUTTON, &MonoAudioTrack::OnBrowse,this);
	
	Track::SetReferenceToTimeTickVector(timeTickVector);
}

//function to set bounds for variable to change as well as number of ticks to draw
void MonoAudioTrack::SetupAxisForVariable(double& start, double& end, double& resolution, int& numTick)
{
	verticalStart = start;
	verticalEnd = end;
	verticalNumTicks = numTick;
	verticalResolution = resolution;
	
	
	//setup tick marks
	if(m_channel_track != nullptr )
	{
		m_channel_track->SetupAxisForVariable(verticalStart,verticalEnd,verticalResolution,verticalNumTicks);
	}
	
}

void MonoAudioTrack::OnBrowse(wxCommandEvent& event)
{
	if(audio_data_stream.GetSize() == 0)
	{
		wxFileDialog fileDlg(this, _("Choose the WAV file"), wxEmptyString, wxEmptyString, _("WAV file|*.wav|All files|*.*"));
		if (fileDlg.ShowModal() == wxID_OK)
		{
			wxString path = fileDlg.GetPath();
			//use this path in your app
			inputSoundFilePath = std::string(path.mb_str());
			
			std::cout << "Input Sound file path:" << inputSoundFilePath << std::endl;
			
			std::cout << "Stream sound file path: " << streamSoundFilePath << std::endl;
			
			int channels = m_channel_track->GetNumberOfChannelsInAudioFile(inputSoundFilePath,input_sfinfo);
			std::cout << "channels:" << channels << std::endl;
			
			
			
			
			if(channels == 1)
			{
				//create a copy of file to reference for editing
				//also put data into stream
				m_channel_track->ReadAndCopyDataFromInputFile(&audio_data_input_copy,inputSoundFilePath,input_sfinfo);
				m_channel_track->CopyInputDataIntoAudioDataStream(&audio_data_input_copy,&audio_data_stream,streamSoundFilePath,input_sfinfo);
				//graph all data in channel to one graph
				m_channel_track->PlotOneChannelStreamAudioDataToGraph(&audio_data_stream,input_sfinfo);
				
				//open file to play during streaming
				audioPlayerPtr->OpenPlayerFile(streamSoundFilePath.c_str());
			}
			
			
			 
		} 
	}
	  
}
	
void MonoAudioTrack::OnSize(wxSizeEvent& event)
{
	Refresh();

	FitInside();
}

void MonoAudioTrack::OnScroll(wxScrollEvent& event)
{
	//std::cout << "Scroll called! \n";
	Refresh(); //for wxDc onPaint stuff 
	
	FitInside(); //for scroll and sizer
}


void MonoAudioTrack::OnPaint(wxPaintEvent& event)
{
	//std::cout << "Current Time in Track:" << *current_time_pos_pointer << std::endl;
	wxPaintDC dc(this);
	
	MonoAudioTrack::render(dc);
}

void MonoAudioTrack::OnLeftMouseClick(wxMouseEvent& event)
{
	
}

void MonoAudioTrack::OnRightMouseClick(wxCommandEvent& event)
{
	
}

double MonoAudioTrack::GetCurrentTime(){return Track::GetCurrentTime();}

void MonoAudioTrack::SetFunctionToCallAfterVariableChange(std::function < void() > thisFunction)
{
	func_after_var_change = thisFunction;
} 
    
void MonoAudioTrack::render(wxDC& dc){}
void MonoAudioTrack::logic_left_click(){}
void MonoAudioTrack::logic_right_click(){}

void MonoAudioTrack::SetReferenceToPlaybackControls(PlaybackControls* controls){playbackControlsPtr = controls;}
PlaybackControls* MonoAudioTrack::GetReferenceToPlaybackControls(){return playbackControlsPtr;}
