#include "soundproducer-track.h"

SoundProducerTrack::SoundProducerTrack(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext) : Track(title)
{
	soundProducerToManipulatePtr = nullptr;
	m_combo_box = nullptr;
	
	//initialize audio player
	audioPlayer = new OpenALSoftPlayer();
	audioPlayer->SetReferenceToAudioContext(thisAudioContext);
	audioPlayer->SetReferenceToAudioDevice(thisAudioDevice);
	
	audioPlayer->InitBuffersForStreaming();
	
	//initialize tracks
	xTrack = new DoubleTrack("X Track");
	yTrack = new DoubleTrack("Y Track");
	zTrack = new DoubleTrack("Z Track");
	audioTrack = new StereoAudioTrack("Track");
	
	audioTrack->SetReferenceToAudioPlayer(audioPlayer);
	
	//initialize track source
	alGenSources(1, &track_source);
	alSourcei(track_source, AL_SOURCE_RELATIVE, AL_TRUE);
	assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source.");
	
	audioTrack->SetReferenceToSourceToManipulate(&track_source);
	
	std::string filepath_stream = "../src/timeline-track-editor/resources/" + title.ToStdString() + ".wav";
	audioTrack->SetStreamAudioFilePath(filepath_stream);
	
	tempX=0.0; tempY=0.0; tempZ=0.0;
	
	xTrack->SetReferenceToVarToManipulate(&tempX);
	yTrack->SetReferenceToVarToManipulate(&tempY);
	zTrack->SetReferenceToVarToManipulate(&tempZ);
	
	
	Connect(wxEVT_PAINT, wxPaintEventHandler(Track::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(Track::OnSize));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleTrack::OnLeftMouseClick));
	Connect(wxEVT_CONTEXT_MENU, wxCommandEventHandler(DoubleTrack::OnRightMouseClick));
	
}

void SoundProducerTrack::FunctionToCallInPlayState()
{
	//std::cout << "FunctionToCall called in SoundProducerTrack \n";
	
	audioTrack->FunctionToCallInPlayState();
	xTrack->FunctionToCallInPlayState();
	yTrack->FunctionToCallInPlayState();
	zTrack->FunctionToCallInPlayState();
		
	if(soundProducerToManipulatePtr != nullptr)
	{
		if(track_source != 0)
		{
			soundProducerToManipulatePtr->SetPositionX(tempX);
			soundProducerToManipulatePtr->SetPositionY(tempY);
			soundProducerToManipulatePtr->SetPositionZ(tempZ);
		}
	}
}

void SoundProducerTrack::FunctionToCallInPauseState()
{
	if(soundProducerToManipulatePtr != nullptr)
	{
		if(track_source != 0)
		{
			audioTrack->FunctionToCallInPauseState();
		}
		
	}
}

void SoundProducerTrack::FunctionToCallInRewindState()
{
	if(soundProducerToManipulatePtr != nullptr)
	{
		if(track_source != 0)
		{
			audioTrack->FunctionToCallInRewindState();
		}
	}
}

void SoundProducerTrack::FunctionToCallInFastForwardState()
{
	if(soundProducerToManipulatePtr!= nullptr)
	{
		if(track_source != 0)
		{
			audioTrack->FunctionToCallInFastForwardState();
		}
	}
}

void SoundProducerTrack::FunctionToCallInNullState()
{
	if(soundProducerToManipulatePtr != nullptr)
	{
		if(track_source != 0)
		{
			audioTrack->FunctionToCallInNullState();
		}
	}
	
}

void SoundProducerTrack::SetReferenceToSoundProducerToManipulate(SoundProducer* thisSoundProducer)
{
	soundProducerToManipulatePtr = thisSoundProducer;
}

StereoAudioTrack* SoundProducerTrack::GetReferenceToStereoAudioTrack(){return audioTrack;}
DoubleTrack* SoundProducerTrack::GetReferenceToXTrack(){return xTrack;}
DoubleTrack* SoundProducerTrack::GetReferenceToYTrack(){return yTrack;}
DoubleTrack* SoundProducerTrack::GetReferenceToZTrack(){return zTrack;}

void SoundProducerTrack::SetReferenceToSoundProducerRegistry(SoundProducerRegistry* thisRegistry){soundproducer_registry_ptr = thisRegistry;}

void SoundProducerTrack::UpdateComboBoxListFromSoundProducerRegistry()
{
	//clear current list and append new one from sound producer registry
	m_combo_box->Clear();
	m_combo_box->Append(soundproducer_registry_ptr->GetSoundProducersToEditList());
}

wxComboBox* SoundProducerTrack::GetReferenceToComboBox(){return m_combo_box;}

void SoundProducerTrack::OnSelectedSoundProducerInComboBox(wxCommandEvent& event)
{
	//std::cout << "Selected sound producer called! \n";
	if(m_combo_box != nullptr)
	{
		std::string thisStringName = (m_combo_box->GetStringSelection()).ToStdString();
	
		SoundProducer* thisSoundProducer = soundproducer_registry_ptr->GetPointerToSoundProducerWithThisName(thisStringName);
		
		thisSoundProducer->SetReferenceToTrackSource(&track_source);
		SoundProducerTrack::SetReferenceToSoundProducerToManipulate(thisSoundProducer);
		
	}
	
}

void SoundProducerTrack::InitTrack(wxWindow* parent, std::vector <int> *timeTickVector)
{		
	//Add a combo box to select soundproducers
	m_combo_box = new wxComboBox(parent, wxID_ANY,"", wxPoint(20,50),wxSize(100,30));
	
	m_combo_box->Bind (wxEVT_COMBOBOX, &SoundProducerTrack::OnSelectedSoundProducerInComboBox,this);
}	

void SoundProducerTrack::SetupAxisForVariable(double& start, double& end,double& resolution, int& numTick)
{
	xTrack->SetupAxisForVariable(start,end,resolution,numTick);
	yTrack->SetupAxisForVariable(start,end,resolution,numTick);
	zTrack->SetupAxisForVariable(start,end,resolution,numTick);
	
}

void SoundProducerTrack::SetupAxisForAudio(double& start, double& end,double& resolution, int& numTick)
{
	audioTrack->SetupAxisForVariable(start,end,resolution,numTick);
}

void SoundProducerTrack::SetReferenceToPlaybackControls(PlaybackControls* controls)
{
	audioTrack->SetReferenceToPlaybackControls(controls);
}

void SoundProducerTrack::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	
	xTrack->render(dc);
	yTrack->render(dc);
	zTrack->render(dc);
	
}

void SoundProducerTrack::OnScroll(wxScrollEvent& event)
{
	Refresh();
	
	FitInside();
}

void SoundProducerTrack::OnSize(wxSizeEvent& event)
{
	Refresh();
	
	FitInside();
}
		
void SoundProducerTrack::SetReferenceToCurrentTimeVariable(double* thisTimeVariable)
{
	Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);
	
	xTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	yTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	zTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
}

void SoundProducerTrack::SetReferenceToTimeTickVector(std::vector <int> *thisVector)
{
	Track::SetReferenceToTimeTickVector(thisVector);
	xTrack->SetReferenceToTimeTickVector(thisVector);
	yTrack->SetReferenceToTimeTickVector(thisVector);
	zTrack->SetReferenceToTimeTickVector(thisVector);
}

std::vector <int> *SoundProducerTrack::GetReferenceToTimeTickVector(){return Track::GetReferenceToTimeTickVector();}

double SoundProducerTrack::GetCurrentTime(){return Track::GetCurrentTime();}
		
void SoundProducerTrack::OnLeftMouseClick(wxMouseEvent& event)
{
	xTrack->logic_left_click();
	yTrack->logic_left_click();
	zTrack->logic_left_click();
	event.Skip();
}

void SoundProducerTrack::OnRightMouseClick(wxCommandEvent& event)
{
	xTrack->logic_right_click();
	yTrack->logic_right_click();
	zTrack->logic_right_click();
	event.Skip();
}
