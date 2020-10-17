#include "RecordingStreamer.h"

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


#include <unistd.h>

RecordingStreamer::RecordingStreamer()
{
	
	m_source_ptr = nullptr;
	m_buffer = 0;
	
	m_deviceIndex = 0;
	m_deviceName = "";
	
	m_playback_device_ptr = nullptr;
	m_playback_context_ptr = nullptr;
	
	m_record_audio_device = nullptr;
	
	//set to default format and sample rate for now
	sampleRate = 48000;
	format = AL_FORMAT_MONO16;
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
    
	
	bufferFrames = BUFFER_FRAMES; // 256 sample frames
	
	stream_opened = false;
	
	
	//setup the array buffer filename endings
	//tempArrayOne.filename_end = "_buf1";
	//tempArrayTwo.filename_end = "_buf2";
	//tempArrayThree.filename_end = "_buf3";
	//tempArrayFour.filename_end = "_buf4";
	
	buffers_generated = false;
}


RecordingStreamer::~RecordingStreamer()
{
	if(m_buffer != 0)
	{
		alDeleteBuffers(1,&m_buffer);
		alDeleteBuffers(NUM_STREAM_BUFFERS,buffers);
	}
	
}

static void al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = nsec / 1000000000ul;
    ts.tv_nsec = nsec % 1000000000ul;
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR){ts = rem;}
}

void RecordingStreamer::SetAsAudioDeviceToRecord(std::string devname, int devIndex)
{
	m_deviceName = devname;
	m_deviceIndex = devIndex;
	
	if(stream_opened)
	{
		RecordingStreamer::FreeDeviceFromRecording();
		stream_opened = false;
	}
}


bool RecordingStreamer::PrepareDeviceForRecording()
{    
	file_path_buffer_stat = data_dir_fp + "buffer-stat.txt";
	
    if(!m_playback_context_ptr)
    {
		std::cout << "Set playback context for audio device recorder!\n"; 
		return false;
	}
	
	if(alcMakeContextCurrent(m_playback_context_ptr) == AL_TRUE)
	{
		if(!buffers_generated)
		{
			alGenBuffers(1, &m_buffer);
			alGenBuffers(NUM_STREAM_BUFFERS, buffers);
			
			ALenum err = alGetError();
			if(err != AL_NO_ERROR)
			{
				std::cout << "Failed to generate buffer!\n";
				fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
				return false;
			}
			
		}
		
		
		buffers_generated = true;
	}
	
    
    return true;
}


void RecordingStreamer::RecordAudioFromDevice()
{
	
	if(!m_source_ptr)
	{
		std::cout << "pointer to source is not defined!\n";
		return;
	}
		
	if(alcMakeContextCurrent(m_playback_context_ptr) == AL_TRUE)
	{
		//load data into the buffer
				
		
		
		bool exitFunction = false;
		
		switch(RecordingStreamer::GetStatusOfHelperProgramBuffers())
		{
			case RecordingStreamer::HelperProgramBufferState::NONE:{std::cout << "No state.\n"; exitFunction = true; break;}
			case RecordingStreamer::HelperProgramBufferState::BUFFER_1_READY_READ:{std::cout << "Buffer 1 ready.\n"; break;}
			case RecordingStreamer::HelperProgramBufferState::BUFFER_2_READY_READ:{std::cout << "Buffer 2 ready.\n"; break;}
		}
		
		if(exitFunction){return;}
		
		ALenum err;
		
		//remove buffer from source
		alSourcei(*m_source_ptr, AL_BUFFER, 0); 
		
		SNDFILE * infile;
		/* Open the file and get the first stream from it */
		std::string filename = data_dir_fp + "device_" + std::to_string(m_deviceIndex) + ".wav";
		
		if (! (infile = sf_open (filename.c_str(), SFM_READ, &sfinfo)))
		{
			std::cout << "Unable to open file " << filename << "\n";
			std::string error;
			error.append(sf_strerror(NULL));
			std::cout << error << std::endl;
			return;
		 }
		
		size_t buffer_index;
		for(buffer_index = 0; buffer_index < NUM_STREAM_BUFFERS; buffer_index++)
		{
			std::array <std::int16_t,BUFFER_FRAMES> data_array;
			
			//read audio from file
						
			size_t read_size = 0;
			size_t count = 0;
			
			size_t read_position = buffer_index * int(BUFFER_FRAMES);
			sf_seek(infile, read_position, SEEK_SET);
			while( (read_size = sf_read_short(infile, data_array.data(), data_array.size()) ) != 0)
			{
				//read audio data
			}
			
			
			//attach samples to buffer
			//set buffer data
			int buffer_byte_size = int(BUFFER_FRAMES) * bit_size;
			alBufferData(buffers[buffer_index], format, data_array.data(), buffer_byte_size, sampleRate);
			
			err = alGetError();
			if(err != AL_NO_ERROR)
			{
				std::cout << "Failed to load data into buffer!\n";
				fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
				return;
			}
			
		}
		
		/* Now queue buffer to source */
		alSourceQueueBuffers(*m_source_ptr, buffer_index, buffers);
		
		//close file 
		if(infile != nullptr)
		{
			sf_close (infile);
		}	
	}

}

void RecordingStreamer::PlayAudioRecordedFromDevice()
{
	if(m_source_ptr)
	{
		
		if(!m_playback_context_ptr)
		{
			return;
		}
		
		if(alcMakeContextCurrent(m_playback_context_ptr) == AL_TRUE)
		{
			ALenum err;
			
			//attach new buffer to source if source is defined
			if(m_source_ptr)
			{
				
				//play audio
				ALint state;
			
				if(state != AL_PLAYING && state != AL_PAUSED && state != AL_STOPPED)
				{
					alGetSourcei(*m_source_ptr, AL_SOURCE_STATE,&state);
					alSourcePlay(*m_source_ptr);
					err = alGetError();
					if(err != AL_NO_ERROR)
					{
						fprintf(stderr, "Error in playback in PlayAudioRecordedFromDevice.\n");
						fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
					}
					
				}
			}
			
		}
		
		
	}
	
}


void RecordingStreamer::FreeDeviceFromRecording()
{
	
}

void RecordingStreamer::SetPointerToSource(ALuint* src)
{
	m_source_ptr = src;
}

ALuint* RecordingStreamer::GetPointerToSource()
{
	return m_source_ptr;
}

void RecordingStreamer::SetPointerToPlaybackDevice(ALCdevice* device){m_playback_device_ptr = device;}
void RecordingStreamer::SetPointerToPlaybackContext(ALCcontext* context){m_playback_context_ptr = context;}

void RecordingStreamer::SetPathToDataDirectory(std::string path){data_dir_fp = path;}

RecordingStreamer::HelperProgramBufferState RecordingStreamer::GetStatusOfHelperProgramBuffers()
{
	 std::ifstream rw_stat_file (file_path_buffer_stat.c_str(), std::ifstream::in);
	 std::string line;
	 
	 RecordingStreamer::HelperProgramBufferState state;
	 
	 if (rw_stat_file.is_open())
	 {
		while ( std::getline (rw_stat_file,line) )
		{
		  if(line == "1")
		  {
			  std::cout << "buffer 1 is ready for read.";
			  state = RecordingStreamer::HelperProgramBufferState::BUFFER_1_READY_READ;
		  }
		  else if(line == "2")
		  {
			  std::cout << "buffer 2 is ready for read.";
			  state = RecordingStreamer::HelperProgramBufferState::BUFFER_2_READY_READ;
		  }
		 
		}
	
		rw_stat_file.close();
		
	 }
	 else
	 {
		 std::cout << "Failed to open " << file_path_buffer_stat << std::endl;
		 state = RecordingStreamer::HelperProgramBufferState::NONE;
	 }
	 
	 
	 
	 return state;
}
