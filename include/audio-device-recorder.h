#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include <sndfile.h>

#define NUM_BUFFERS 4

class AudioDeviceRecorder
{
	
public:
	AudioDeviceRecorder();
	~AudioDeviceRecorder();
	
	//function to set an audio device as the audio device to record
	void SetAsAudioDeviceToRecord(std::string devname);
	
	//function to prepare recording of the audio device
	bool PrepareDeviceForRecording();
	
	//function to record audio and putting audio data into a buffer to source
	void RecordAudioFromDevice();
	
	//function to play audio from a buffer
	void PlayAudioRecordedFromDevice();
	
	//function to stop device from recording and clean up some things
	void FreeDeviceFromRecording();
	
	//functions to set and get pointer to source to playback audio
	void SetPointerToSource(ALuint* src);
	ALuint* GetPointerToSource();
	
	//functions to set pointers to playback context and device
	void SetPointerToPlaybackDevice(ALCdevice* device);
	void SetPointerToPlaybackContext(ALCcontext* context);
	
private:
	
	//name of audio device to record
	std::string m_deviceName;
	
	//audio device to record
	ALCdevice* m_audio_device;
	
	//pointer to source to playback audio
	ALuint* m_source_ptr;
	
	//formatting of audio data
	SF_INFO sfinfo;
	
	size_t bit_size;
	size_t frame_size;
	int buffer_time_ms;
	size_t buffer_pack_size;
	ALenum format;
	
	//buffers
	//ALuint buffers[NUM_BUFFERS];
	ALuint m_buffer;
	
	//array holding data to load into buffer
	ALbyte data_samples[22050];
	
	//pointer to device and context used for playback and not recording
	ALCdevice* m_playback_device_ptr;
	ALCcontext* m_playback_context_ptr;
	
};

#endif
