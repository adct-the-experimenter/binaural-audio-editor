#include "audio-device-recorder.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * OpenAL Recording Example
 *
 * Copyright (c) 2017 by Chris Robinson <chris.kcat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

std::int16_t* data_array1_ptr = nullptr;
std::int16_t* data_array2_ptr = nullptr;
std::int16_t* data_array3_ptr = nullptr;
std::int16_t* data_array4_ptr = nullptr;
int* buffer_filled_ptr = nullptr;

AudioDeviceRecorder::AudioDeviceRecorder()
{	
	m_deviceIndex = 0;
	m_deviceName = "";
	
	
	//set to default format and sample rate for now
	sampleRate = 48000;
	int num_channels = 1;
	
	 //formatting for .wav file
    //set to signed 16-bit PCM wav little endian
    sfinfo = {0};
    sfinfo.channels = num_channels; 
    sfinfo.samplerate = sampleRate; 
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; 
    
    bit_size = sizeof(std::int16_t);
    
    frame_size = sfinfo.channels * bit_size;
	buffer_time_ms = 50;
    
    
    buffer_pack_size = (uint64_t)sampleRate * ((double(buffer_time_ms))/1000);
    
	//parameters.deviceId = adc.getDefaultInputDevice();
	parameters.nChannels = num_channels;
	parameters.firstChannel = 0;
	bufferFrames = BUFFER_FRAMES; // 256 sample frames
	
	m_stream_opened = false;
    m_stream_closed = false;
    
    recording = false;
	
	data_array1_ptr = tempArrayOne.array_data.data();
    data_array2_ptr = tempArrayTwo.array_data.data();
	data_array3_ptr = tempArrayThree.array_data.data();
	data_array4_ptr = tempArrayFour.array_data.data();
    
    buffer_filled_ptr = &buffer_filled;
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

static void al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = nsec / 1000000000ul;
    ts.tv_nsec = nsec % 1000000000ul;
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR){ts = rem;}
}

void AudioDeviceRecorder::SetAsAudioDeviceToRecord(std::string devname, int devIndex)
{
	m_deviceName = devname;
	m_deviceIndex = devIndex;
	parameters.deviceId = m_deviceIndex;
	
	if(m_stream_opened)
	{
		AudioDeviceRecorder::FreeDeviceFromRecording();
		m_stream_opened = false;
	}
}


bool AudioDeviceRecorder::PrepareDeviceForRecording()
{
	 if(m_ad_combo_box)
    {
		int devIndex = m_ad_combo_box->GetSelection();
		m_deviceIndex = devIndex;
		
		parameters.deviceId = m_deviceIndex;
		
		return true;
	}
	
    return false;
}



bool new_stream = false;


int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
	if ( status ){std::cout << "Stream overflow detected!" << std::endl;}

	unsigned int i;

	// Do something with the data in the "inputBuffer" buffer.

	std::int16_t* first_index_audio_data_ptr = nullptr;
	std::int16_t* audio_data_ptr = nullptr;
		
	switch(*buffer_filled_ptr)
	{
		case 0:
		{
			first_index_audio_data_ptr = data_array1_ptr;
			break;
		}
		case 1:
		{
			first_index_audio_data_ptr = data_array2_ptr;
			break;
		}
		case 2:
		{
			first_index_audio_data_ptr = data_array3_ptr;
			break;
		}
		case 3:
		{
			first_index_audio_data_ptr = data_array4_ptr;
			break;
		}
		default:{break;}
	}
	
	if(!first_index_audio_data_ptr){return 1;}
	
	audio_data_ptr = first_index_audio_data_ptr;
	
	std::int16_t* my_buffer = (std::int16_t*)inputBuffer;
	
	for ( i=0; i < nBufferFrames; i++ ) 
	{
	    *audio_data_ptr++ = *my_buffer++;
	    
	    //std::cout << "audio data i:" << i << " , " << *audio_data_ptr << std::endl;
	}
	
	//increment buffer filled
	(*buffer_filled_ptr)++;
	
	//reset buffer filled if more than NUM_BUFFERS
	if(*buffer_filled_ptr >= NUM_BUFFERS){*buffer_filled_ptr = 0;}
	
	return 0;
}


void AudioDeviceRecorder::RecordAudioFromDevice()
{

	if(!m_stream_opened)
	{
		int nBuffers = NUM_BUFFERS;
		
		try {
			
		adc.openStream( NULL, &parameters, RTAUDIO_SINT16,
						sampleRate, &bufferFrames, &record );
		
		
		
		double data[2];
		
		
		//start stream
		adc.startStream();
		m_stream_opened = true;
	  }
	  catch ( RtAudioError& e ) {
		e.printMessage();
		return;
	  }
	}
	
	
	//write data to separate file
	
	size_t buffer_index;
	for(buffer_index = 0; buffer_index < NUM_BUFFERS; buffer_index++)
	{
		DataArray* audio_data_ptr = nullptr;
		
		while(buffer_filled != 0 && buffer_filled != buffer_index + 1)
		{
			//wait until buffer index is filled in callback function record
		}
		
		if(buffer_index == 0){audio_data_ptr = &tempArrayOne; /*std::cout << "Getting Buffer 1 data!\n";*/}
		if(buffer_index == 1){audio_data_ptr = &tempArrayTwo; /*std::cout << "Getting Buffer 2 data!\n";*/}
		if(buffer_index == 2){audio_data_ptr = &tempArrayThree; /*std::cout << "Getting Buffer 3 data!\n";*/}
		if(buffer_index == 3){audio_data_ptr = &tempArrayFour; /*std::cout << "Getting Buffer 4 data!\n";*/}
		
		if(!audio_data_ptr){break;}
		
		//write to file
		
		//clear array
		audio_data_ptr->array_data.fill(0);
		
	}
		
	

}

void AudioDeviceRecorder::PlayAudioRecordedFromDevice()
{
	
}


void AudioDeviceRecorder::FreeDeviceFromRecording()
{
	try {
		// Stop the stream
		adc.stopStream();
	}
	catch (RtAudioError& e) {
		e.printMessage();
	}
	if ( adc.isStreamOpen() ){ adc.closeStream();}
	
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
