#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include <sndfile.h>

class AudioDeviceRecorder
{
	
public:
	AudioDeviceRecorder();
	~AudioDeviceRecorder();
	
	//function to set an audio device as the audio device to record
	void SetAsAudioDeviceToRecord(std::string devname);
	
	//function to set path to file to be used for streaming audio
	void SetFilepathToAudioStreamBuffering(std::string filepath);
	
	//function to prepare recording of the audio device
	bool PrepareDeviceForRecording();
	
	//function to start recording audio and putting audio data into file for stream
	void StartRecordingOnDevice();
	
	//function to stop device from recording and clean up some things
	void FreeDeviceFromRecording();
	
private:
	//name of audio device to record
	std::string m_deviceName;
	
	//file path to file used for streaming audio
	//ideally it is set as the file used by sound producer track
	std::string m_stream_sound_filepath;
	
	ALCdevice* m_audio_device;
	
	SNDFILE* stream_filehandle;
	
	//formatting of audio data
	SF_INFO sfinfo;
	
	size_t bit_size;
	ALuint frame_size;
	int buffer_time_ms;
	size_t buffer_pack_size;
	
	
};

#endif
