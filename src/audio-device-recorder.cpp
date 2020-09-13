#include "audio-device-recorder.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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

AudioDeviceRecorder::AudioDeviceRecorder()
{
	
	m_source_ptr = nullptr;
	m_buffer = 0;
	
	m_playback_device_ptr = nullptr;
	m_playback_context_ptr = nullptr;
	
	//set to default format and sample rate for now
	int sample_rate = 44100;
	format = AL_FORMAT_MONO16;
	int num_channels = 1;
	
	 //formatting for .wav file
    //set to signed 16-bit PCM wav little endian
    sfinfo = {0};
    sfinfo.channels = num_channels; 
    sfinfo.samplerate = sample_rate; 
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; 
    
    bit_size = sizeof(int16_t);
    
    frame_size = sfinfo.channels * bit_size;
	buffer_time_ms = 100;
    
    
    buffer_pack_size = (uint64_t)sample_rate * ((double(buffer_time_ms))/1000) * frame_size;
  
}


AudioDeviceRecorder::~AudioDeviceRecorder()
{
	if(m_buffer != 0)
	{
		alDeleteBuffers(1,&m_buffer);
	}
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
	
    
    // Generate the buffers
    /*
    alGenBuffers(NUM_BUFFERS, buffers);
    if(alGetError() == AL_NO_ERROR)
    {
		std::cout << "Failed to generate buffers!\n";
		return false;
	}
    */
    if(!m_playback_context_ptr)
    {
		std::cout << "Set playback context for audio device recorder!\n"; 
		return false;
	}

	
	m_audio_device = alcCaptureOpenDevice(m_deviceName.c_str(), sfinfo.samplerate, format, buffer_pack_size);
    if(!m_audio_device)
    {
        return false;
    }
    
    if (alGetError() != AL_NO_ERROR) {
        return false;
    }
    
    
    return true;
}

void AudioDeviceRecorder::RecordAudioFromDevice()
{
	
	ALuint data_buffer_size = 0;    
	
	ALint sample;
	
	ALenum format = AL_FORMAT_MONO16;
    
    alcCaptureStart(m_audio_device);
    
    int slen = 22050 * bit_size; //get size of data in bytes

    //while (data_buffer_size < buffer_pack_size) {
        
	//get samples
	alcGetIntegerv(m_audio_device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
	alcCaptureSamples(m_audio_device, (ALCvoid *)data_buffer, sample);


	
	
	
	//attach new buffer to source if source is defined
	if(m_source_ptr)
	{
		alSourcei(*m_source_ptr, AL_BUFFER, m_buffer);
	}
    //}

    alcCaptureStop(m_audio_device);
    alcCaptureCloseDevice(m_audio_device);
        

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
			ALuint tempBuffer;
			alGenBuffers(1, &tempBuffer);
			if(alGetError() != AL_NO_ERROR)
			{
				std::cout << "Failed to generate buffer!\n";
				return;
			}
			
			//attach samples to buffer
			//set buffer data
			//alBufferData(buffers[buffer_index], format, &buffer[0], slen, sfinfo.samplerate);
			alBufferData(tempBuffer, format, &data_buffer, 22050, sfinfo.samplerate);
			
			alSourcePlay(*m_source_ptr);
			ALenum err = alGetError();
			if(err != AL_NO_ERROR)
			{
				fprintf(stderr, "Error in playback in PlayAudioRecordedFromDevice.\n");
				fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
				return;
			}
			
			alDeleteBuffers(1,&tempBuffer);
		}
		
		
	}
	
}


void AudioDeviceRecorder::FreeDeviceFromRecording()
{
	ALCenum err;
	
	alcCaptureStop(m_audio_device);
    //fprintf(stderr, "\rCaptured %u samples\n", recorder.mDataSize);
    if(err != ALC_NO_ERROR)
        fprintf(stderr, "Got device error 0x%04x: %s\n", err, alcGetString(m_audio_device, err));

    alcCaptureCloseDevice(m_audio_device);    
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
