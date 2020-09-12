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
	m_stream_sound_filepath = "";
	stream_filehandle = nullptr;
}


AudioDeviceRecorder::~AudioDeviceRecorder()
{
	
}


void AudioDeviceRecorder::SetAsAudioDeviceToRecord(std::string devname)
{
	m_deviceName = devname;
}

void AudioDeviceRecorder::SetFilepathToAudioStreamBuffering(std::string filepath)
{
	m_stream_sound_filepath = filepath;
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
	if(m_stream_sound_filepath == ""){return false;}
    
    ALCenum err;
    
	//set to default format and sample rate for now
	int sample_rate = 44100;
	ALenum format = AL_FORMAT_MONO16;
	int bufferSize = 32768;
	int num_channels = 1;
	
	m_audio_device = alcCaptureOpenDevice(m_deviceName.c_str(), sample_rate, format, bufferSize);
    if(!m_audio_device)
    {
        return false;
    }
    
    if (alGetError() != AL_NO_ERROR) {
        return false;
    }
    
    
    //formatting for .wav file
    //set to signed 16-bit PCM wav little endian
    sfinfo = {0};
    sfinfo.channels = num_channels; 
    sfinfo.samplerate = sample_rate; 
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; 
    
    frame_size = sfinfo.channels * bit_size;
	buffer_time_ms = 100;
    
    bit_size = sizeof(int16_t);
    buffer_pack_size = (uint64_t)sample_rate * (double(buffer_time_ms))/1000 * frame_size;
  
    
    return true;
}

void AudioDeviceRecorder::StartRecordingOnDevice()
{
	ALCenum err;
	
	ALuint data_buffer_size = 0;    
    
    ALbyte buffer[22050];
	
	ALint sample;
	
	// Open the stream file for writing
	if (! ( stream_filehandle = sf_open (m_stream_sound_filepath.c_str(), SFM_WRITE, &sfinfo) ) )
	{	
		std::cout << "Not able to open stream file for writing" << stream_filehandle << std::endl;
		puts (sf_strerror (NULL)) ;
		alcCaptureCloseDevice(m_audio_device);
		return;
	} 
    
    alcCaptureStart(m_audio_device);

    while (data_buffer_size < buffer_pack_size) {
        alcGetIntegerv(m_audio_device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
        alcCaptureSamples(m_audio_device, (ALCvoid *)buffer, sample);

		//write buffer of data to file

		sf_count_t write_count = 0; 		
		sf_seek(stream_filehandle, 0, SEEK_SET);
		
		write_count = sf_write_raw(stream_filehandle, buffer, frame_size);
		data_buffer_size += write_count; 
    }

    alcCaptureStop(m_audio_device);
    alcCaptureCloseDevice(m_audio_device);
        
	
    
    //close file
    sf_close(stream_filehandle);
}


void AudioDeviceRecorder::FreeDeviceFromRecording()
{
	ALCenum err;
	
	alcCaptureStop(m_audio_device);
    //fprintf(stderr, "\rCaptured %u samples\n", recorder.mDataSize);
    if(err != ALC_NO_ERROR)
        fprintf(stderr, "Got device error 0x%04x: %s\n", err, alcGetString(m_audio_device, err));

    alcCaptureCloseDevice(m_audio_device);
    
    sf_close(stream_filehandle);
}
