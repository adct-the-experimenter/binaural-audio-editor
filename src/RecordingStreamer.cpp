#include "RecordingStreamer.h"

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
	
	data_array1_ptr = tempArrayOne.array_data.data();
    data_array2_ptr = tempArrayTwo.array_data.data();
	data_array3_ptr = tempArrayThree.array_data.data();
	data_array4_ptr = tempArrayFour.array_data.data();
    
    buffer_filled_ptr = &buffer_filled;
}


RecordingStreamer::~RecordingStreamer()
{
	if(m_buffer != 0)
	{
		alDeleteBuffers(1,&m_buffer);
		alDeleteBuffers(NUM_BUFFERS,buffers);
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
	
    if(!m_playback_context_ptr)
    {
		std::cout << "Set playback context for audio device recorder!\n"; 
		return false;
	}
	
	if(alcMakeContextCurrent(m_playback_context_ptr) == AL_TRUE)
	{
		alGenBuffers(1, &m_buffer);
		alGenBuffers(NUM_BUFFERS, buffers);
		
		ALenum err = alGetError();
		if(err != AL_NO_ERROR)
		{
			std::cout << "Failed to generate buffer!\n";
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			return false;
		}
		
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
		
		//m_audio_data_saved.swap(tempArray);
		
		ALenum err;
		
		//remove buffer from source
		alSourcei(*m_source_ptr, AL_BUFFER, 0); 
		
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
			//attach samples to buffer
			//set buffer data
			//alBufferData(buffers[buffer_index], format, &buffer[0], slen, sfinfo.samplerate);
			int buffer_byte_size = int(BUFFER_FRAMES) * bit_size;
			alBufferData(buffers[buffer_index], format, audio_data_ptr->array_data.data(), buffer_byte_size, sampleRate);
			
			//clear array
			audio_data_ptr->array_data.fill(0);
			
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
