#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include <sndfile.h>

#include <array>

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#define NUM_BUFFERS 4
#define BUFFER_FRAMES 2400

class AudioDeviceRecorder
{
	
public:
	AudioDeviceRecorder();
	~AudioDeviceRecorder();
	
	//function to set an audio device as the audio device to record
	void SetAsAudioDeviceToRecord(std::string devname, int devIndex);
	
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
	
	//RtAudio parameters
	RtAudio adc;
	RtAudio::StreamParameters parameters;
	unsigned int sampleRate;
	unsigned int bufferFrames; // 256 sample frames

	//name of audio device to record
	std::string m_deviceName;
	int m_deviceIndex;
	
	//audio device to record
	ALCdevice* m_record_audio_device;
	
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
	ALuint buffers[NUM_BUFFERS];
	ALuint m_buffer;
	
	
	//pointer to device and context used for playback and not recording
	ALCdevice* m_playback_device_ptr;
	ALCcontext* m_playback_context_ptr;
	
	std::array <std::int16_t,BUFFER_FRAMES> m_audio_data_saved;
	
	bool stream_opened;
	
};

#endif
