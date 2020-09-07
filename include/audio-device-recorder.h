#ifndef AUDIO_DEVICE_RECORDER_H
#define AUDIO_DEVICE_RECORDER_H

#include <string>

class AudioDeviceRecorder
{
	
public:
	AudioDeviceRecorder();
	~AudioDeviceRecorder();
	
	//function to set an audio device as the audio device to record
	void SetAsAudioDeviceToRecord(std::string devname);
	
	//function to set path to file to be used for streaming audio
	void SetFilepathToAudioStreamBuffering(std::string filepath);
	
private:
	//name of audio device to record
	std::string m_deviceName;
	
	//file path to file used for streaming audio
	//ideally it is set as the file used by sound producer track
	std::string m_stream_sound_filepath;
	
};

#endif
