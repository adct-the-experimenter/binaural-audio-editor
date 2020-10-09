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

#define BUFFER_FRAMES 512

AudioDeviceRecorder::AudioDeviceRecorder()
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
    
	//parameters.deviceId = adc.getDefaultInputDevice();
	parameters.nChannels = num_channels;
	parameters.firstChannel = 0;
	bufferFrames = BUFFER_FRAMES; // 256 sample frames
	
	stream_opened = false;
    
}


AudioDeviceRecorder::~AudioDeviceRecorder()
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

void AudioDeviceRecorder::SetAsAudioDeviceToRecord(std::string devname, int devIndex)
{
	m_deviceName = devname;
	m_deviceIndex = devIndex;
	parameters.deviceId = m_deviceIndex;
	
	if(stream_opened)
	{
		AudioDeviceRecorder::FreeDeviceFromRecording();
		stream_opened = false;
	}
}


bool AudioDeviceRecorder::PrepareDeviceForRecording()
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


int buffer_filled = 0;

std::array <std::int16_t,BUFFER_FRAMES> tempArrayOne;
std::array <std::int16_t,BUFFER_FRAMES> tempArrayTwo;
std::array <std::int16_t,BUFFER_FRAMES> tempArrayThree;
std::array <std::int16_t,BUFFER_FRAMES> tempArrayFour;

bool new_stream = false;

static int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
	if ( status ){std::cout << "Stream overflow detected!" << std::endl;}

	unsigned int i;

	// Do something with the data in the "inputBuffer" buffer.

	std::int16_t* first_index_audio_data_ptr = nullptr;
	std::int16_t* audio_data_ptr = nullptr;
	
	int buffer_to_fill = 0;
	
	switch(buffer_filled)
	{
		case 0:
		{
			first_index_audio_data_ptr = tempArrayOne.data();
			buffer_to_fill = 1;
			break;
		}
		case 1:
		{
			first_index_audio_data_ptr = tempArrayTwo.data();
			buffer_to_fill = 2;
			break;
		}
		case 2:
		{
			first_index_audio_data_ptr = tempArrayThree.data();
			buffer_to_fill = 3;
			break;
		}
		case 3:
		{
			first_index_audio_data_ptr = tempArrayFour.data();
			buffer_to_fill = 4;
			break;
		}
		default:{break;}
	}
	
	if(!first_index_audio_data_ptr){return 1;}
	
	audio_data_ptr = first_index_audio_data_ptr;
	
	for ( i=0; i < nBufferFrames; i++ ) 
	{
	    *audio_data_ptr++ = *(std::int16_t*)inputBuffer++;
	    
	    //std::cout << "audio data i:" << i << " , " << *audio_data_ptr << std::endl;
	}
	
	//increment buffer filled
	buffer_filled++;
	
	//reset buffer filled if more than NUM_BUFFERS
	if(buffer_filled >= NUM_BUFFERS){buffer_filled = 0;}
	
	return 0;
}


void AudioDeviceRecorder::RecordAudioFromDevice()
{

	if(!stream_opened)
	{
		int nBuffers = NUM_BUFFERS;
		
		try {
			
		adc.openStream( NULL, &parameters, RTAUDIO_SINT16,
						sampleRate, &bufferFrames, &record );
		
		
		
		double data[2];
		
		
		//start stream
		adc.startStream();
		stream_opened = true;
	  }
	  catch ( RtAudioError& e ) {
		e.printMessage();
		return;
	  }
	}
	
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
			std::int16_t* audio_data_ptr = nullptr;
			
			while(buffer_index == 0 && buffer_filled != buffer_index + 1)
			{
				//wait until buffer index is filled in callback function record
			}
			
			if(buffer_index == 0){audio_data_ptr = tempArrayOne.data(); std::cout << "Getting Buffer 1 data!\n";}
			if(buffer_index == 1){audio_data_ptr = tempArrayTwo.data(); std::cout << "Getting Buffer 2 data!\n";}
			if(buffer_index == 2){audio_data_ptr = tempArrayThree.data(); std::cout << "Getting Buffer 3 data!\n";}
			if(buffer_index == 3){audio_data_ptr = tempArrayFour.data(); std::cout << "Getting Buffer 4 data!\n";}
			
			if(!audio_data_ptr){break;}
			//attach samples to buffer
			//set buffer data
			//alBufferData(buffers[buffer_index], format, &buffer[0], slen, sfinfo.samplerate);
			int buffer_byte_size = int(BUFFER_FRAMES) * bit_size;
			alBufferData(buffers[buffer_index], format, audio_data_ptr, buffer_byte_size, sampleRate);
			
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
/*		
		//attach samples to buffer
		//set buffer data
		//alBufferData(buffers[buffer_index], format, &buffer[0], slen, sfinfo.samplerate);
		int buffer_byte_size = m_audio_data_saved.size() * sizeof(std::int16_t);
		alBufferData(m_buffer, format, tempArrayCopy.data(), buffer_byte_size, sampleRate);
		
		err = alGetError();
		if(err != AL_NO_ERROR)
		{
			std::cout << "Failed to load data into buffer!\n";
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			return;
		}
		
		//attach buffer to source
		
		alSourcei(*m_source_ptr, AL_BUFFER, m_buffer);
	}
	
    */
	}

}

void AudioDeviceRecorder::PlayAudioRecordedFromDevice()
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

void AudioDeviceRecorder::SetPointerToSource(ALuint* src)
{
	m_source_ptr = src;
}

ALuint* AudioDeviceRecorder::GetPointerToSource()
{
	return m_source_ptr;
}

void AudioDeviceRecorder::SetPointerToPlaybackDevice(ALCdevice* device){m_playback_device_ptr = device;}
void AudioDeviceRecorder::SetPointerToPlaybackContext(ALCcontext* context){m_playback_context_ptr = context;}
