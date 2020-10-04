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


//local pointer used to store a sample of audio data to array
std::int16_t* audio_data_ptr = nullptr;

AudioDeviceRecorder::AudioDeviceRecorder()
{
	
	m_source_ptr = nullptr;
	m_buffer = 0;
	
	m_playback_device_ptr = nullptr;
	m_playback_context_ptr = nullptr;
	
	m_record_audio_device = nullptr;
	
	//point to first index of audio data array
	audio_data_ptr = m_audio_data_saved.data();
	
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
    
    bit_size = sizeof(int16_t);
    
    frame_size = sfinfo.channels * bit_size;
	buffer_time_ms = 50;
    
    
    buffer_pack_size = (uint64_t)sampleRate * ((double(buffer_time_ms))/1000);
    
	//parameters.deviceId = adc.getDefaultInputDevice();
	parameters.nChannels = num_channels;
	parameters.firstChannel = 0;
	bufferFrames = 256; // 256 sample frames
	
	stream_opened = false;
    
}


AudioDeviceRecorder::~AudioDeviceRecorder()
{
	if(m_buffer != 0)
	{
		alDeleteBuffers(1,&m_buffer);
	}
}

static void al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = nsec / 1000000000ul;
    ts.tv_nsec = nsec % 1000000000ul;
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR){ts = rem;}
}

void AudioDeviceRecorder::SetAsAudioDeviceToRecord(std::string devname)
{
	m_deviceName = devname;
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


static int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  if ( status ){std::cout << "Stream overflow detected!" << std::endl;}
  
  unsigned int i;
  
  // Do something with the data in the "inputBuffer" buffer.
  
  std::int16_t* first_index_audio_data_ptr = audio_data_ptr;
  
  for ( i=0; i<nBufferFrames; i++ ) 
  {
     *audio_data_ptr++ = *(std::int16_t*)inputBuffer++;
  }
  
  audio_data_ptr = first_index_audio_data_ptr; 
  return 0;
}


void AudioDeviceRecorder::RecordAudioFromDevice()
{

	if(!stream_opened)
	{
		try {
		adc.openStream( NULL, &parameters, RTAUDIO_SINT16,
						sampleRate, &bufferFrames, &record );
		adc.startStream();
		stream_opened = true;
	  }
	  catch ( RtAudioError& e ) {
		e.printMessage();
		return;
	  }
	}
  
  
	//std::cout << data_samples[100] << std::endl;
	
	if(!m_source_ptr)
	{
		std::cout << "pointer to source is not defined!\n";
		return;
	}
	
	if(alcMakeContextCurrent(m_playback_context_ptr) == AL_TRUE)
	{
		//load data into the buffer
		
		ALenum err;
		
		//remove buffer from source
		alSourcei(*m_source_ptr, AL_BUFFER, 0); 
		
		//attach samples to buffer
		//set buffer data
		//alBufferData(buffers[buffer_index], format, &buffer[0], slen, sfinfo.samplerate);
		int buffer_byte_size = buffer_pack_size * bit_size;
		alBufferData(m_buffer, format, &m_audio_data_saved[0], buffer_byte_size, sfinfo.samplerate);
		
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
	
    
    //alcCaptureCloseDevice(m_record_audio_device);
        

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
	ALCenum err;
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
