#ifndef RECORDING_STREAMER_H
#define RECORDING_STREAMER_H

#include <string>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include <sndfile.h>

#include <array>


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdint>

//sample rate * time
#define BUFFER_FRAMES 600
#define NUM_STREAM_BUFFERS 4

//class used to periodically load new data from several files and pass it into an OpenAL buffer

struct DataArray
{
	std::array <std::int16_t,BUFFER_FRAMES> array_data;
	bool filled = false;
	std::string filename_end = "";
};

class RecordingStreamer
{
	
public:
	RecordingStreamer();
	~RecordingStreamer();
	
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
	
	//function to set path to data directory
	void SetPathToDataDirectory(std::string path);
	
	enum class HelperProgramBufferState : std::int8_t { NONE=0, BUFFER_1_READY_READ, BUFFER_2_READY_READ};
	
private:
	
	
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
	ALuint buffers[NUM_STREAM_BUFFERS];
	ALuint m_buffer;
	
	
	//pointer to device and context used for playback and not recording
	ALCdevice* m_playback_device_ptr;
	ALCcontext* m_playback_context_ptr;
	
	bool stream_opened;
	
	std::string data_dir_fp;
	
	bool buffers_generated;
	
	//file indicating status of audio recorder helper program buffers
	std::string file_path_buffer_stat;
	RecordingStreamer::HelperProgramBufferState GetStatusOfHelperProgramBuffers();
	
	
};

#endif
