#include "audio-device-recorder.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

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


AudioDataQueue::AudioDataQueue()
{
	//start current position at zero
	m_main_current_pos = 0;
	
	//set full bool to false
	m_full = false;
	
	m_data_in_backup_copy = false;
	
	 //formatting for .wav file
    //set to signed 16-bit PCM wav little endian
    
    int sampleRate = 48000;
	int num_channels = 1;
	
    sfinfo = {0};
    sfinfo.channels = num_channels; 
    sfinfo.samplerate = sampleRate; 
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; 
    
}

AudioDataQueue::~AudioDataQueue()
{
	
}

void AudioDataQueue::WriteArrayDataToFile(std::string filename)
{
	//write to file
	AudioDataQueue::SetWritingLockState(true);
	
	SNDFILE* outFile;
	
	//std::cout << "Writing to file " << filename << std::endl;
	
	// Open the stream file
	if (! ( outFile = sf_open (filename.c_str(), SFM_WRITE, &sfinfo)))
	{	
		std::cout << "Not able to open stream file for writing " << outFile << std::endl;
		puts (sf_strerror (NULL)) ;
		return;
	}
	
	//write data to file
	size_t readSize = m_main_array.size();
	sf_count_t write_count = 0; 
	size_t count_buffer = 0;
	
	sf_seek(outFile, 0, SEEK_SET);
	write_count = sf_write_short(outFile, &m_main_array.front(), readSize);
	
	sf_close(outFile);
	
	//reset main current position
	m_main_current_pos = 0;
	
	//reset main array full bool
	AudioDataQueue::SetMainArrayFullBool(false);
	
	AudioDataQueue::SetWritingLockState(false);
}

//function to copy data from an array to the main array
void AudioDataQueue::CopyDataToMainArray(std::array <std::int16_t,BUFFER_FRAMES> &this_array)
{
	
	//if there is data in the backup copy array
	if(m_data_in_backup_copy)
	{
		
		size_t input_size = this_array.size();
	
		size_t start = m_main_current_pos;
	
		size_t endIndex = this_array.size();
	
		for(size_t i = 0; i < endIndex; i++)
		{
			m_main_array[start + i] = this_array[i];
		}
		
		m_data_in_backup_copy = false;
	}
	
	size_t input_size = this_array.size();
	
	size_t start = m_main_current_pos;
	
	size_t endIndex = this_array.size();
	
	//if there is more data left over than can be filled in array
	if(m_main_current_pos + input_size >= m_main_array.size())
	{
		//set end based on what can be read to main array
		endIndex = input_size - (m_main_array.size() - m_main_current_pos);
		
		AudioDataQueue::SetMainArrayFullBool(true);
		m_data_in_backup_copy = true;
	}
	
	//increment main array current reading position
	m_main_current_pos = m_main_current_pos + input_size;
	
	for(size_t i = 0; i < endIndex; i++)
	{
		m_main_array[start + i] = this_array[i];
	} 
	
}

void AudioDataQueue::SetMainArrayFullBool(bool state){m_full = state;}
bool AudioDataQueue::IsMainArrayFull(){return m_full;}

std::int16_t* AudioDataQueue::GetPointerToCurrentReadPosition(){return &m_main_array[m_main_current_pos];}

void AudioDataQueue::SetWritingLockState(bool state){m_write_lock = state;}
bool AudioDataQueue::GetWritingLockState(){return m_write_lock;}

void AudioDataQueue::IncrementReadPosition(){m_main_current_pos++;}

AudioDataQueue* ptrToAudioDataQueue = nullptr;

void AudioDataQueue::CheckIfMainArrayFilled()
{
	if(m_main_current_pos >= m_main_array.size() - 1)
	{
		AudioDataQueue::SetMainArrayFullBool(true);
	}
}

AudioDeviceRecorder::AudioDeviceRecorder()
{	
	m_deviceIndex = 0;
	m_deviceName = "";
	
	m_state = AudioDeviceRecorder::HelperProgramBufferState::NONE;
	
	//set to default format and sample rate for now
	sampleRate = 96000;
	int num_channels = 1;
	
    bit_size = sizeof(std::int16_t);
    
    frame_size = num_channels * bit_size;
	buffer_time_ms = 50;
    
    
    buffer_pack_size = (uint64_t)sampleRate * ((double(buffer_time_ms))/1000);
    
	//parameters.deviceId = adc.getDefaultInputDevice();
	parameters.nChannels = num_channels;
	parameters.firstChannel = 0;
	bufferFrames = BUFFER_FRAMES;
	
	m_stream_opened = false;
    m_stream_closed = false;
    
    recording = false;
	    
    //Set up file path for the data directory
    std::string datadir; 
	
	datadir = DATADIR_STR;
	
	#ifdef WIN32
	datadir = "../../src/timeline-track-editor/resources/";
	#endif
	
	if(datadir == ""){datadir = "../../src/timeline-track-editor/resources/";}
	
	data_dir_fp = datadir;
	
	fp_state_file = data_dir_fp + "buffer-stat.txt";
	fp_rlock_file = data_dir_fp + "to-helper-msg.txt";
	
	//setup recorder timer
	std::function< void() > func = std::bind(&AudioDeviceRecorder::RecordAudioFromDevice, this);
	m_rec_timer.AddFunctionToTimerLoop(func);
	
	ptrToAudioDataQueue = &m_main_audio_queue;
	
	//time repeat interval (number of buffer frames / sample rate)*(1000 milliseconds / seconds) + 10 milliseconds
	int repeat_interval = ((double(BUFFER_FRAMES) / sampleRate) * 1000) + 10;
	
	m_rec_timer.SetRepeatInterval(repeat_interval);
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


int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
	if ( status ){std::cout << "Stream overflow detected!" << std::endl;}

	if(ptrToAudioDataQueue->GetWritingLockState())
	{
		return 0;
	}

	// Do something with the data in the "inputBuffer" buffer.
	
	std::int16_t* audio_data_ptr = nullptr;	
	
	audio_data_ptr = ptrToAudioDataQueue->GetPointerToCurrentReadPosition();
	
	if(!audio_data_ptr){return 1;}
	
	std::int16_t* my_buffer = (std::int16_t*)inputBuffer;
		
	unsigned int i = 0;
	for ( i=0; i < nBufferFrames; i++ ) 
	{
		ptrToAudioDataQueue->IncrementReadPosition();
		ptrToAudioDataQueue->CheckIfMainArrayFilled();
		
		if(ptrToAudioDataQueue->IsMainArrayFull()){break;}
	    
	    *audio_data_ptr++ = *my_buffer++;
	    
	    //std::cout << "audio data i:" << i << " , " << *audio_data_ptr << std::endl;
	}
	
	return 0;
}

static void CopyFile(std::string inputFile, std::string outputFile)
{
	std::ifstream src;
	std::ofstream dst;

	src.open(inputFile.c_str(), std::ios::in | std::ios::binary);
	dst.open(outputFile.c_str(), std::ios::out | std::ios::binary);
	dst << src.rdbuf();

	src.close();
	dst.close();
}


void AudioDeviceRecorder::RecordAudioFromDevice()
{

	if(!m_stream_opened)
	{
		
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
	
	AudioDeviceRecorder::SetState(AudioDeviceRecorder::HelperProgramBufferState::NONE);
	
	recording = true;
	
	if(AudioDeviceRecorder::IsFileInReadLock()){std::cout << "File is locked for reading.\n"; return;}
	
	//if main audio data array is full
	if(ptrToAudioDataQueue->IsMainArrayFull())
	{
		
		std::string filename = data_dir_fp + "device_" + std::to_string(m_deviceIndex) + ".wav";
		
		if(AudioDeviceRecorder::IsFileInReadLock()){std::cout << "File is locked for reading.\n"; return;}
		
		//write data to file
		m_main_audio_queue.WriteArrayDataToFile(filename);
		
		AudioDeviceRecorder::SetState(AudioDeviceRecorder::HelperProgramBufferState::BUFFER_1_READY_READ);
		AudioDeviceRecorder::WriteStateToFile();
		
	}
	else
	{
		AudioDeviceRecorder::SetState(AudioDeviceRecorder::HelperProgramBufferState::NONE);
	}
		
		
}

void AudioDeviceRecorder::PlayAudioRecordedFromDevice()
{
	
}


void AudioDeviceRecorder::FreeDeviceFromRecording()
{
	AudioDeviceRecorder::SetState(AudioDeviceRecorder::HelperProgramBufferState::NONE);
	AudioDeviceRecorder::WriteStateToFile();
	
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
	if(!recording)
	{
		recording = true;
		
		if(!AudioDeviceRecorder::PrepareDeviceForRecording())
		{
			wxMessageBox("Failed to prepare device for recording! \n");
		}
		
		m_rec_timer.start();
	}
	
}

void AudioDeviceRecorder::OnSelectedAudioDeviceInComboBox(wxCommandEvent& event)
{
		
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

void AudioDeviceRecorder::SetState(AudioDeviceRecorder::HelperProgramBufferState state){m_state = state;}
AudioDeviceRecorder::HelperProgramBufferState AudioDeviceRecorder::GetState(){return m_state;}

void AudioDeviceRecorder::WriteStateToFile()
{
	std::ofstream state_outfile;
	
	state_outfile.open(fp_state_file.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(state_outfile.is_open())
	{
		int state = 0;
		switch(AudioDeviceRecorder::GetState())
		{
			case AudioDeviceRecorder::HelperProgramBufferState::NONE:{state = 0; break;}
			case AudioDeviceRecorder::HelperProgramBufferState::BUFFER_1_READY_READ:{state = 1; break;}
			case AudioDeviceRecorder::HelperProgramBufferState::BUFFER_2_READY_READ:{state = 2; break;}
		}
		
		state_outfile << state;
	}
	
	state_outfile.close();
}

bool AudioDeviceRecorder::IsFileInReadLock()
{
	std::string line;
	std::ifstream lock_file (fp_rlock_file.c_str(), std::ifstream::in);
			 
	int inputchoice = 0;
	 
	if (lock_file.is_open())
	{
		while ( std::getline (lock_file,line) )
		{
			inputchoice = std::stoi(line);
		}
		
		lock_file.close();
	}
	
	if(inputchoice == 0)
	{
		return false;
	}
	else if(inputchoice == 1)
	{
		return true;
	}
}

//Recording Timer

RecorderTimer::RecorderTimer() : wxTimer()
{
	
}

void RecorderTimer::Notify()
{
	m_function();
	al_nssleep(100);
}

void RecorderTimer::start()
{
	//time repeat interval (number of buffer frames / sample rate) + 10 milliseconds
	
    wxTimer::Start(m_repeat_interval,wxTIMER_CONTINUOUS); //the timer calls Notify every TIMER_INTERVAL milliseconds
}

void RecorderTimer::stop()
{
	wxTimer::Stop();
}

void RecorderTimer::AddFunctionToTimerLoop( std::function < void() > thisFunction)
{
	m_function = thisFunction;
}

void RecorderTimer::SetRepeatInterval(int interval){m_repeat_interval = interval;}
