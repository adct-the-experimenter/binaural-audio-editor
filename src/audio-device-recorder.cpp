#include "audio-device-recorder.h"

#include <iostream>

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
    
    
    //formatting for .wav file
    //set to signed 16-bit PCM wav little endian
    
    sfinfo.channels = num_channels; 
    sfinfo.samplerate = sample_rate; 
    sfinfo.format = SF_FORMAT_WAV & SF_FORMAT_PCM_16; 
    
    frame_size = sfinfo.channels * bit_size;
	buffer_time_ms = 100;
    
    bit_size = sizeof(int16_t);
    buffer_pack_size = (uint64_t)sample_rate * (double(buffer_time_ms))/1000 * frame_size;
    
	// Open the stream file for writing
	if (! ( stream_filehandle = sf_open (m_stream_sound_filepath.c_str(), SFM_WRITE, &sfinfo)))
	{	
		std::cout << "Not able to open stream file for writing" << stream_filehandle << std::endl;
		puts (sf_strerror (NULL)) ;
		alcCaptureCloseDevice(m_audio_device);
		return false;
	} 
    
    return true;
}

void AudioDeviceRecorder::StartRecordingOnDevice()
{
	ALCenum err;
	
	int sample_rate = 44100;
	
	
	ALbyte* data_buffer = nullptr;
	
	ALuint data_buffer_size = 0;
	
	alcCaptureStart(m_audio_device);
    while(
		  (double)data_buffer_size < buffer_pack_size 
		   &&
          (err=alcGetError(m_audio_device)) == ALC_NO_ERROR
          )
    {
        ALCint count = 0;
        fprintf(stderr, "\rCaptured %u samples", data_buffer_size);
        alcGetIntegerv(m_audio_device, ALC_CAPTURE_SAMPLES, 1, &count);
        if(count < 1)
        {
            al_nssleep(10000000);
            continue;
        }
        if(count > buffer_pack_size)
        {
            ALbyte *data = calloc(frame_size, (ALuint)count);
            free(data_buffer);
            data_buffer = data;
            data_buffer_size = count;
        }
        alcCaptureSamples(m_audio_device, data_buffer, count);
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
        /* Byteswap multibyte samples on big-endian systems (wav needs little-
         * endian, and OpenAL gives the system's native-endian).
         */
        if(bit_size == 16)
        {
            ALCint i;
            for(i = 0;i < count*recorder.mChannels;i++)
            {
                ALbyte b = data_buffer[i*2 + 0];
                data_buffer[i*2 + 0] = data_buffer[i*2 + 1];
                data_buffer[i*2 + 1] = b;
            }
        }
        else if(bit_size == 32)
        {
            ALCint i;
            for(i = 0;i < count*recorder.mChannels;i++)
            {
                ALbyte b0 = data_buffer[i*4 + 0];
                ALbyte b1 = data_buffer[i*4 + 1];
                data_buffer[i*4 + 0] = recorder.mBuffer[i*4 + 3];
                data_buffer[i*4 + 1] = recorder.mBuffer[i*4 + 2];
                data_buffer[i*4 + 2] = b1;
                data_buffer[i*4 + 3] = b0;
            }
        }
#endif
        data_buffer_size += (ALuint)fwrite(data_buffer, recorder.mFrameSize, (ALuint)count,
                                             stream_filehandle);
    }
    
    //stop audio device capture
    alcCaptureStop(m_audio_device);
    
    //write data to file
	size_t readSize = frame_size;
	sf_count_t write_count = 0; 
	size_t count_buffer = 0;
	
	sf_seek(stream_filehandle, 0, SEEK_SET);
	write_count = sf_write_double(stream_filehandle, data_buffer, readSize);
	
	
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
