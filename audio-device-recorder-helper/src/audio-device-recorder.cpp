#include "audio-device-recorder.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


 /*
	PortAudio code adapted from pa_fuzz.c sample code http://portaudio.com/docs/v19-doxydocs/pa__fuzz_8c_source.html
    7  * $Id$
    8  *
    9  * This program uses the PortAudio Portable Audio Library.
   10  * For more information see: http://www.portaudio.com
   11  * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
   12  *
   13  * Permission is hereby granted, free of charge, to any person obtaining
   14  * a copy of this software and associated documentation files
   15  * (the "Software"), to deal in the Software without restriction,
   16  * including without limitation the rights to use, copy, modify, merge,
   17  * publish, distribute, sublicense, and/or sell copies of the Software,
   18  * and to permit persons to whom the Software is furnished to do so,
   19  * subject to the following conditions:
   20  *
   21  * The above copyright notice and this permission notice shall be
   22  * included in all copies or substantial portions of the Software.
   23  *
   24  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   25  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   26  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   27  * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
   28  * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
   29  * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   30  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   31  
   32 
   33 
   34  * The text above constitutes the entire PortAudio license; however, 
   35  * the PortAudio community also makes the following non-binding requests:
   36  *
   37  * Any person wishing to distribute modifications to the Software is
   38  * requested to send the modifications to the original developer so that
   39  * they can be incorporated into the canonical version. It is also 
   40  * requested that these non-binding requests be included along with the 
   41  * license above.
   42  
*/

std::int16_t* audio_data_ptr = nullptr;

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
    
    audio_data_ptr = m_audio_data_saved.data();
    
    m_stream_opened = false;
    m_stream_closed = false;
    
    recording = false;
    
    m_stream_src_ptr = nullptr;
}


AudioDeviceRecorder::~AudioDeviceRecorder()
{
	//if stream was opened and not closed
	if(m_stream_opened && !m_stream_closed)
	{
		AudioDeviceRecorder::FreeDeviceFromRecording();
	}
	
	m_stream_opened = false;
	m_stream_closed = false;
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

static int gNumNoInputs = 0;

 /* This routine will be called by the PortAudio engine when audio is needed.
   ** It may be called at interrupt level on some machines so don't do anything
    ** that could mess up the system like calling malloc() or free().
    */



static int writeAudioToFileCallback( const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData )
{
	
   float *out = (float*)(outputBuffer);
   const float *in = (const float*)(inputBuffer);
   unsigned int i;
   (void) timeInfo; /* Prevent unused variable warnings. */
   (void) statusFlags;
   (void) userData;

   if( inputBuffer == NULL )
   {
	   for( i = 0; i < framesPerBuffer; i++ )
	   {
		   *out++ = 0;  /* left - silent */
		   //*out++ = 0;  /* right - silent */
	   }
	   gNumNoInputs += 1;
   }
   else
   {
	   
	   std::int16_t* first_index_audio_data_ptr = audio_data_ptr;
	   
	   for( i = 0; i < framesPerBuffer; i++ )
	   {
		   //*out++ = FUZZ(*in++);  /* left - distorted */
		   *out++ = *in++;          /* right - clean */
		   
		   if(audio_data_ptr != nullptr)
		   {
			   *audio_data_ptr = std::int16_t(*in);
				std::cout << "audio data i:" << *audio_data_ptr << std::endl;
		   
				audio_data_ptr++;
		   }
		   
	   }
	   
	   audio_data_ptr = first_index_audio_data_ptr;
	}
   
	return paContinue;
}


bool AudioDeviceRecorder::PrepareDeviceForRecording()
{    
    if(m_ad_combo_box)
    {
		int devIndex = m_ad_combo_box->GetSelection();

		err = Pa_Initialize();
		if( err != paNoError )
		{
			printf("Unable to initialize portaudio!\n");
			printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
			return false;
		}

		inputParameters.device = devIndex; /* default input device */
		m_device_index = devIndex;
		if (inputParameters.device == paNoDevice) 
		{
			printf("Unable to set input!\n");
			fprintf(stderr,"Error:Invalid input device.\n");
			return false;
		}
		inputParameters.channelCount = 2;       /* mono input */
		inputParameters.sampleFormat = paInt16;
		inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;

		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (outputParameters.device == paNoDevice) 
		{
			printf("Unable to set output!\n");
			fprintf(stderr,"Error: No default output device.\n");
			return false;
		}
		outputParameters.channelCount = 2;       /* stereo output */
		outputParameters.sampleFormat = paInt16;
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		
		
		return true;
	}
	
    return false;
}

void AudioDeviceRecorder::RecordAudioFromDevice()
{
	//open stream for recording
	err = Pa_OpenStream(
			 &m_stream_src_ptr,
			 &inputParameters,
			 &outputParameters,
			 SAMPLE_RATE,
			 FRAMES_PER_BUFFER,
			 0, 
			 writeAudioToFileCallback,
			 NULL );
			 
	if( err != paNoError )
	{
		printf("Unable to open stream!\n");
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return;
	}
	else
	{
		m_stream_opened = true;
		std::cout << "Sucessfully opened stream!\n";
	}
	
	//start recording stream
	
	err = Pa_StartStream( m_stream_src_ptr );
	if( err != paNoError )
	{
		printf("Unable to start stream!\n");
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return;
	}
	else
	{
		std::cout << "Successfully started stream for recording!\n";
		recording = true;
	}
	
}


void AudioDeviceRecorder::FreeDeviceFromRecording()
{
	err = Pa_CloseStream( m_stream_src_ptr );
	if( err != paNoError )
	{
		printf("Unable to close stream!\n");
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
	}
	else
	{
		m_stream_closed = true;
	}

	printf("Finished. gNumNoInputs = %d\n", gNumNoInputs );
	Pa_Terminate();
}

wxComboBox* AudioDeviceRecorder::GetPointerToComboBox(){return m_ad_combo_box;}

wxButton* AudioDeviceRecorder::GetPointerToRecordButton(){return m_record_button_ptr;}

wxButton* AudioDeviceRecorder::GetPointerToStopButton(){return m_stop_button_ptr;}

void AudioDeviceRecorder::InitTrack(wxWindow* parent)
{
	this->Create(parent, wxID_ANY, wxPoint(wxDefaultPosition.x,wxDefaultPosition.y), wxSize(200, 100),wxTAB_TRAVERSAL,wxT("t1"));
	this->Show();
	
	//add record button
	m_record_button_ptr = new wxButton(parent, wxID_ANY, wxT("Record"), wxDefaultPosition, wxSize(100, 30) );
    m_record_button_ptr->Bind(wxEVT_BUTTON, &AudioDeviceRecorder::OnRecordButtonPressed,this);
    
    //Add a combo box to select audio devices
	m_ad_combo_box = new wxComboBox(parent, wxID_ANY,"", wxPoint(20,50),wxSize(300,30));
	m_ad_combo_box->Bind (wxEVT_COMBOBOX, &AudioDeviceRecorder::OnSelectedAudioDeviceInComboBox,this);
	
	//stop button
	m_stop_button_ptr = new wxButton(parent, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxSize(100, 30) );
    m_stop_button_ptr->Bind(wxEVT_BUTTON, &AudioDeviceRecorder::OnStopButtonPressed,this);
}

void AudioDeviceRecorder::OnRecordButtonPressed(wxCommandEvent& event)
{
	AudioDeviceRecorder::RecordAudioFromDevice();
}

void AudioDeviceRecorder::OnSelectedAudioDeviceInComboBox(wxCommandEvent& event)
{
	if(!AudioDeviceRecorder::PrepareDeviceForRecording())
	{
		wxMessageBox("Failed to prepare device for recording! \n");
	}	
}

void AudioDeviceRecorder::OnStopButtonPressed(wxCommandEvent& event)
{
	if(recording)
	{
		recording = false;
	
		AudioDeviceRecorder::FreeDeviceFromRecording();
	}
}
