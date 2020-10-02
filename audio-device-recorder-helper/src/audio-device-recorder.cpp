#include "audio-device-recorder.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "portaudio.h"


AudioDeviceRecorder::AudioDeviceRecorder()  : wxPanel()
{
	
	//set to default format and sample rate for now
	int sample_rate = 48000;
	int num_channels = 1;
	
	m_device_index = -1;
	
	 //formatting for .wav file
    //set to signed 16-bit PCM wav little endian
    sfinfo = {0};
    sfinfo.channels = num_channels; 
    sfinfo.samplerate = sample_rate; 
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; 
    
    bit_size = sizeof(int16_t);
    
    frame_size = sfinfo.channels * bit_size;
	buffer_time_ms = 50;
    
    buffer_pack_size = (uint64_t)sample_rate * ((double(buffer_time_ms))/1000);
    
   
}


AudioDeviceRecorder::~AudioDeviceRecorder()
{
	
	
}


void AudioDeviceRecorder::SetAsAudioDeviceToRecord(std::string devname)
{
	m_deviceName = devname;
}

static void al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = nsec / 1000000000ul;
    ts.tv_nsec = nsec % 1000000000ul;
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR){ts = rem;}
}


bool AudioDeviceRecorder::PrepareDeviceForRecording()
{    
    if(m_ad_combo_box)
    {
		int devIndex = m_ad_combo_box->GetSelection() ;

		PaStreamParameters inputParameters, outputParameters;
		PaStream *stream;
		PaError err;

		err = Pa_Initialize();
		if( err != paNoError ){}

		inputParameters.device = devIndex; /* default input device */
		if (inputParameters.device == paNoDevice) {
		 fprintf(stderr,"Error: No default input device.\n");
		}
		inputParameters.channelCount = 2;       /* stereo input */
		inputParameters.sampleFormat = PA_SAMPLE_TYPE;
		inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;

		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (outputParameters.device == paNoDevice) {
		 fprintf(stderr,"Error: No default output device.\n");
		}
		outputParameters.channelCount = 2;       /* stereo output */
		outputParameters.sampleFormat = PA_SAMPLE_TYPE;
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		
		//err = Pa_OpenStream(
		//		 &stream,
		//		 &inputParameters,
		//		 &outputParameters,
		//		 SAMPLE_RATE,
		//		 FRAMES_PER_BUFFER,
		//		 0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
		//		 fuzzCallback,
		//		 NULL );
		if( err != paNoError )

		return true;
	}
	
    return false;
}



void AudioDeviceRecorder::RecordAudioFromDevice()
{
        
}


void AudioDeviceRecorder::FreeDeviceFromRecording()
{

}

wxComboBox* AudioDeviceRecorder::GetPointerToComboBox(){return m_ad_combo_box;}

wxButton* AudioDeviceRecorder::GetPointerToRecordButton(){return m_record_button_ptr;}

void AudioDeviceRecorder::InitTrack(wxWindow* parent)
{
	this->Create(parent, wxID_ANY, wxPoint(wxDefaultPosition.x,wxDefaultPosition.y), wxSize(200, 100),wxTAB_TRAVERSAL,wxT("t1"));
	this->Show();
	
	//add record button
	m_record_button_ptr = new wxButton(parent, wxID_ANY, wxT("Record"), wxDefaultPosition, wxSize(200, 30) );
    m_record_button_ptr->Bind(wxEVT_BUTTON, &AudioDeviceRecorder::OnRecordButtonPressed,this);
    
    //Add a combo box to select audio devices
	m_ad_combo_box = new wxComboBox(parent, wxID_ANY,"", wxPoint(20,50),wxSize(100,30));
	m_ad_combo_box->Bind (wxEVT_COMBOBOX, &AudioDeviceRecorder::OnSelectedAudioDeviceInComboBox,this);
}

void AudioDeviceRecorder::OnRecordButtonPressed(wxCommandEvent& event)
{
	
}

void AudioDeviceRecorder::OnSelectedAudioDeviceInComboBox(wxCommandEvent& event)
{
	if(!AudioDeviceRecorder::PrepareDeviceForRecording())
	{
		wxMessageBox("Failed to prepare device for recording! \n");
	}
	
}
