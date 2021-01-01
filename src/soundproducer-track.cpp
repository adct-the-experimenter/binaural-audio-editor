#include "soundproducer-track.h"


#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)


#ifndef DATADIR
    #define DATADIR "../src/timeline-track-editor/resources/"
#endif

#define DATADIR_NAME STR(DATADIR)

std::string DATADIR_STR = DATADIR_NAME;

#include <unistd.h>

SoundProducerTrack::SoundProducerTrack(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext) : Track(title)
{
	track_source = 0;
	
	soundProducerToManipulatePtr = nullptr;
	m_combo_box = nullptr;
	
	m_importAudioDAWButton = nullptr;
	
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
	audioTrack->SetTrackOption(StereoAudioTrack::Options::ONLY_BUFFER_AUDIO);

	//initialize track source
	alGenSources(1, &track_source);
	alSourcei(track_source, AL_SOURCE_RELATIVE, AL_FALSE);
	assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source.");
	
	audioTrack->SetReferenceToSourceToManipulate(&track_source);
	
	std::string datadir; 
	
	datadir = DATADIR_STR;
	
	#ifdef WIN32
	datadir = "../src/timeline-track-editor/resources/";
	#endif
	
	std::string filepath_stream = datadir + title.ToStdString() + ".wav";
	audioTrack->SetStreamAudioFilePath(filepath_stream);
	streamSoundFilePath = filepath_stream;
	
	tempX=0.0; tempY=0.0; tempZ=0.0;
	
	xTrack->SetReferenceToVarToManipulate(&tempX);
	yTrack->SetReferenceToVarToManipulate(&tempY);
	zTrack->SetReferenceToVarToManipulate(&tempZ);
	
	//initialize reverb applied status
	SoundProducerTrack::SetStatusReverbApplied(false);
	
	m_saveData.time_value_map_x_ptr = xTrack->GetPointerToTimeValueMap();
	m_saveData.time_value_map_y_ptr = yTrack->GetPointerToTimeValueMap();
	m_saveData.time_value_map_z_ptr = zTrack->GetPointerToTimeValueMap();
	
	
	Connect(wxEVT_PAINT, wxPaintEventHandler(Track::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(Track::OnSize));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleTrack::OnLeftMouseClick));
	Connect(wxEVT_CONTEXT_MENU, wxCommandEventHandler(DoubleTrack::OnRightMouseClick));
	
}

SoundProducerTrack::~SoundProducerTrack()
{
	if(track_source != 0)
	{
		alDeleteSources(1, &track_source);
	}
}

void SoundProducerTrack::FunctionToCallInPlayState()
{
	//std::cout << "FunctionToCall called in SoundProducerTrack \n";
	
	//change position
	xTrack->FunctionToCallInPlayState();
	yTrack->FunctionToCallInPlayState();
	zTrack->FunctionToCallInPlayState();
		
	if(soundProducerToManipulatePtr != nullptr)
	{
		if(track_source != 0)
		{
			if(!soundProducerToManipulatePtr->GetFreeRoamBool())
			{
				soundProducerToManipulatePtr->SetPositionX(tempX);
				soundProducerToManipulatePtr->SetPositionY(tempY);
				soundProducerToManipulatePtr->SetPositionZ(tempZ);
			}
			
		}
	}
	
	//buffer audio
	audioTrack->FunctionToCallInPlayState();
}

void SoundProducerTrack::BufferAndPlayAudio(double& current_time)
{
	audioTrack->BufferAndPlayAudio(current_time);
}

void SoundProducerTrack::StopAudio()
{
	audioTrack->StopAudio();
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
	m_saveData.soundproducer_name = soundProducerToManipulatePtr->GetNameString();
}

StereoAudioTrack* SoundProducerTrack::GetReferenceToStereoAudioTrack(){return audioTrack;}
DoubleTrack* SoundProducerTrack::GetReferenceToXTrack(){return xTrack;}
DoubleTrack* SoundProducerTrack::GetReferenceToYTrack(){return yTrack;}
DoubleTrack* SoundProducerTrack::GetReferenceToZTrack(){return zTrack;}

void SoundProducerTrack::SetReferenceToSoundProducerRegistry(SoundProducerRegistry* thisRegistry){soundproducer_registry_ptr = thisRegistry;}

void SoundProducerTrack::UpdateComboBoxListFromSoundProducerRegistry()
{
	//get current name selected
	std::string thisStringName = (m_combo_box->GetStringSelection()).ToStdString();
		
	//clear current list and append new one from sound producer registry
	m_combo_box->Clear();
	m_combo_box->Append(soundproducer_registry_ptr->GetSoundProducersToEditList());
	
	if(thisStringName != "")
	{
		SoundProducerTrack::SelectSoundProducerByName(thisStringName);
	}
	
}

wxComboBox* SoundProducerTrack::GetReferenceToComboBox(){return m_combo_box;}

void SoundProducerTrack::OnSelectedSoundProducerInComboBox(wxCommandEvent& event)
{
	//std::cout << "Selected sound producer called! \n";
	if(m_combo_box != nullptr)
	{
		std::string thisStringName = (m_combo_box->GetStringSelection()).ToStdString();
		
		SoundProducerTrack::SelectSoundProducerByName(thisStringName);
		
	}
	
}

void SoundProducerTrack::SelectSoundProducerByName(std::string name)
{
	SoundProducer* thisSoundProducer = soundproducer_registry_ptr->GetPointerToSoundProducerWithThisName(name);
	
	if(thisSoundProducer)
	{
		m_saveData.soundproducer_name = name;
		
		thisSoundProducer->SetReferenceToTrackSource(&track_source);
		SoundProducerTrack::SetReferenceToSoundProducerToManipulate(thisSoundProducer);
		
		tempX = thisSoundProducer->GetPositionX();
		tempY = thisSoundProducer->GetPositionY();
		tempZ = thisSoundProducer->GetPositionZ();
		
		if(soundproducer_registry_ptr != nullptr)
		{
			soundproducer_registry_ptr->RemoveThisNameFromAllComboBoxesExceptThisOne(name,m_combo_box);
		}
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
	
	event.Skip();
	
}

void SoundProducerTrack::OnScroll(wxScrollEvent& event)
{
	Refresh();
	
	FitInside();
	
	event.Skip();
}

void SoundProducerTrack::OnSize(wxSizeEvent& event)
{
	Refresh();
	
	FitInside();
	
	event.Skip();
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

ALuint* SoundProducerTrack::GetReferenceToTrackSource(){return &track_source;}

SoundProducer* SoundProducerTrack::GetReferenceToSoundProducerManipulated(){return soundProducerToManipulatePtr;}

bool SoundProducerTrack::IsReverbApplied(){return reverbApplied;}

void SoundProducerTrack::SetStatusReverbApplied(bool status){reverbApplied = status;}


void SoundProducerTrack::SetReferenceToImportAudioDAWButton(wxButton* thisButton)
{
	m_importAudioDAWButton = thisButton;
	
	m_importAudioDAWButton->Bind(wxEVT_BUTTON, &SoundProducerTrack::OnImportAudioDAWButtonClick,this);
}

void SoundProducerTrack::OnImportAudioDAWButtonClick(wxCommandEvent& event)
{
	//clear graphs
	audioTrack->GetReferenceToLeftChannelTrack()->ClearGraph();
	audioTrack->GetReferenceToRightChannelTrack()->ClearGraph();
	
	std::cout << "Importing audio from DAW...\n";
	
	//load audio file imported_audio_DAW.wav
	//use this path in your app
	std::string inputSoundFilePath = std::string("../src/timeline-track-editor/resources/") + std::string("imported_audio_DAW.wav");
	
	m_saveData.soundfilepath = inputSoundFilePath;
	
	SF_INFO input_sfinfo;
	
	int channels = audioTrack->GetReferenceToLeftChannelTrack()->GetNumberOfChannelsInAudioFile(inputSoundFilePath,input_sfinfo);
	std::cout << "channels:" << channels << std::endl;
	
	if(channels == 0)
	{
		wxMessageBox(wxT("Failed to load audio from imported_audio_DAW.wav!\n No such file or data is incorrect. \n"));
	}
	
	//object to hold audio data for streaming
	AudioStreamContainer audio_data_stream;
	
	//Hold data for left channel and right channel
	std::vector <double> audio_data_input_copy;
	
	if(channels == 1)
	{
		//create a copy of file to reference for editing
		//also put data into stream
		audioTrack->GetReferenceToLeftChannelTrack()->ReadAndCopyDataFromInputFile(&audio_data_input_copy,inputSoundFilePath,input_sfinfo);
		audioTrack->GetReferenceToLeftChannelTrack()->CopyInputDataIntoAudioDataStream(&audio_data_input_copy,&audio_data_stream,streamSoundFilePath,input_sfinfo);
		//graph all data in channel to one graph
		audioTrack->GetReferenceToLeftChannelTrack()->PlotOneChannelStreamAudioDataToGraph(&audio_data_stream,input_sfinfo);
	}
	else if(channels == 2)
	{
		//create a copy of file to reference for editing
		//also put data into stream
		audioTrack->GetReferenceToLeftChannelTrack()->ReadAndCopyDataFromInputFile(&audio_data_input_copy,inputSoundFilePath,input_sfinfo);
		audioTrack->GetReferenceToLeftChannelTrack()->CopyInputDataIntoAudioDataStream(&audio_data_input_copy,&audio_data_stream,streamSoundFilePath,input_sfinfo);
			
		//plot left channel data to one graph 
		audioTrack->GetReferenceToLeftChannelTrack()->PlotLeftChannelStreamAudioDataToGraph(&audio_data_stream,input_sfinfo);
		//plot right channel data to other graph
		audioTrack->GetReferenceToRightChannelTrack()->PlotRightChannelStreamAudioDataToGraph(&audio_data_stream,input_sfinfo);
		
	}
	
	//open file to play during streaming
	audioPlayer->OpenPlayerFile(streamSoundFilePath.c_str());
	
	std::cout << "\nFinished importing audio from DAW!\n";
	event.Skip();
}

SoundProducerTrackSaveData SoundProducerTrack::GetSoundProducerTrackSaveData()
{
	//work around for getting file path info from stereo track for saving
	m_saveData.soundfilepath = audioTrack->GetInputSoundFilePath();
	
	return m_saveData;
}

void SoundProducerTrack::LoadSoundProducerTrackSaveData(SoundProducerTrackSaveData& data)
{
	m_saveData = data;
	
	m_saveData.time_value_map_x_ptr = xTrack->GetPointerToTimeValueMap();
	m_saveData.time_value_map_y_ptr = yTrack->GetPointerToTimeValueMap();
	m_saveData.time_value_map_z_ptr = zTrack->GetPointerToTimeValueMap();
	
	if(data.time_value_map_x_ptr)
	{
		xTrack->LoadDataFromThisTimeValueMap(*data.time_value_map_x_ptr);
		delete data.time_value_map_x_ptr;
		data.time_value_map_x_ptr = nullptr;
	}
	
	if(data.time_value_map_y_ptr)
	{
		yTrack->LoadDataFromThisTimeValueMap(*data.time_value_map_y_ptr);
		delete data.time_value_map_y_ptr;
		data.time_value_map_y_ptr = nullptr;
	}
	
	if(data.time_value_map_z_ptr)
	{
		zTrack->LoadDataFromThisTimeValueMap(*data.time_value_map_z_ptr);
		delete data.time_value_map_z_ptr;
		data.time_value_map_z_ptr = nullptr;
	}
	
	//load from sound file path
	if(m_saveData.soundfilepath != "")
	{
		audioTrack->LoadAudioFromFileToTrack(m_saveData.soundfilepath);
	}
}

void SoundProducerTrack::SetComboBoxToThisSelectionName(std::string name)
{
	int select_index = m_combo_box->FindString(name,true); //case sensitive = true
	m_combo_box->SetSelection(select_index);
}
