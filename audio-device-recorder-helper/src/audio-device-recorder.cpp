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


#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)


#ifndef DATADIR
    #define DATADIR "../../src/timeline-track-editor/resources/"
#endif

#define DATADIR_NAME STR(DATADIR)

std::string DATADIR_STR = DATADIR_NAME;

//pointers for record function to use
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
	
	//set pointers for the stati record callback function to use
	data_array1_ptr = tempArrayOne.array_data.data();
    data_array2_ptr = tempArrayTwo.array_data.data();
	data_array3_ptr = tempArrayThree.array_data.data();
	data_array4_ptr = tempArrayFour.array_data.data();
    
    buffer_filled_ptr = &buffer_filled;
    
    //Set up file path for the data directory
    std::string datadir; 
	
	datadir = DATADIR_STR;
	
	#ifdef WIN32
	datadir = "../../src/timeline-track-editor/resources/";
	#endif
	
	if(datadir == ""){datadir = "../../src/timeline-track-editor/resources/";}
	
	data_dir_fp = datadir;
	
	//setup the array buffer filename endings
	tempArrayOne.filename_end = "_buf1";
	tempArrayTwo.filename_end = "_buf2";
	tempArrayThree.filename_end = "_buf3";
	tempArrayFour.filename_end = "_buf4";
	
	//setup recorder timer
	std::function< void() > func = std::bind(&AudioDeviceRecorder::RecordAudioFromDevice, this);
	m_rec_timer.AddFunctionToTimerLoop(func);
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
		std::string devName = m_ad_combo_box->GetStringSelection().ToStdString();
		
		AudioDeviceRecorder::SetAsAudioDeviceToRecord(devName,devIndex);
		
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
	recording = true;
	
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
		
		SNDFILE * outFile;
		
		std::string filename = data_dir_fp + "device_" + std::to_string(m_deviceIndex) + audio_data_ptr->filename_end + ".wav";
		std::cout << "Writing to file " << filename << std::endl;
		
		// Open the stream file
		if (! ( outFile = sf_open (filename.c_str(), SFM_WRITE, &sfinfo)))
		{	
			std::cout << "Not able to open stream file for writing" << outFile << std::endl;
			puts (sf_strerror (NULL)) ;
			return;
		} 
		
		//write data
		size_t readSize = audio_data_ptr->array_data.size();
		sf_count_t write_count = 0; 
		size_t count_buffer = 0;
		
		sf_seek(outFile, 0, SEEK_SET);
		write_count = sf_write_short(outFile, &audio_data_ptr->array_data.front(), readSize);
		
		sf_close(outFile);
	
		//clear array
		audio_data_ptr->array_data.fill(0);
		
	}
		
	std::cout << "Finished writing to file!\n";
	al_nssleep(1000);
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
	m_rec_timer.start();
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
		m_rec_timer.stop();
		
		recording = false;
		
		if(m_stream_opened)
		{
			AudioDeviceRecorder::FreeDeviceFromRecording();
		}
		
	}
}

//Playback Timer

RecorderTimer::RecorderTimer() : wxTimer()
{
	
}

void RecorderTimer::Notify()
{
	m_function();
}

void RecorderTimer::start()
{
	int time_repeat_interval = 20;// in milliseconds
    wxTimer::Start(time_repeat_interval,wxTIMER_CONTINUOUS); //the timer calls Notify every TIMER_INTERVAL milliseconds
}

void RecorderTimer::stop()
{
	wxTimer::Stop();
}

void RecorderTimer::AddFunctionToTimerLoop( std::function < void() > thisFunction)
{
	m_function = thisFunction;
}
