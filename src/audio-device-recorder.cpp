#include "audio-device-recorder.h"


#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"


AudioDeviceRecorder::AudioDeviceRecorder()
{
	
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
