#ifndef OPENALSOFT_PLAYER
#define OPENALSOFT_PLAYER


#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cassert>
#include <cmath>

#include "time.h"

#include "sndfile.h"

#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft

#define NUM_BUFFERS 4
#define BUFFER_TIME_MS 200 // 200 milliseconds 
#define MAX_CHANNELS 2

class OpenALSoftPlayer
{
public:
	OpenALSoftPlayer();
	~OpenALSoftPlayer();
	
	//Initialize OpenAL Soft system
	bool InitOpenALSoft(ALCdevice* thisAudioDevice, ALCcontext* thisAudioContext);
	
	//function to initialize buffers to play for streaming
	void InitBuffersForStreaming();
	
	//function to initialize external source 
	void InitSource(ALuint* source);
	
	//functions to set reference to audio device and audio context to use for player
	void SetReferenceToAudioDevice(ALCdevice* thisAudioDevice);
	void SetReferenceToAudioContext(ALCcontext* thisAudioContext);
	
	//functions to manipulate source
	void PlaySource(ALuint* thisSource);
	void PauseSource(ALuint* thisSource);
	void RewindSource(ALuint* thisSource);
	void StopSource(ALuint* thisSource);
	
	
	//function to stream source
	void StreamSource(ALuint* thisSource);
	
	//function to open file for streaming audio
	int OpenPlayerFile(const char *filename);
	
	//function to close current file loaded for streaming audio
	void ClosePlayerFile();
	
	int StartPlayer(ALuint* source,double& current_time);
	
	int UpdatePlayer(ALuint* source,double& current_time);
	
	void CloseOpenALSoft(ALCdevice* thisAudioDevice, ALCcontext* thisAudioContext);
	
private:
	ALCdevice* audioDevicePtr; //pointer to audio device to be used
    ALCcontext* alContextPtr; //pointer to context of where audio is played
    
    ALuint buffers[NUM_BUFFERS];
	//size of temporary buffers to read
	uint32_t buffer_size;

    /* The format of the output stream */
    ALenum format;
	ALsizei sample_rate;
	
	//libsndfile file handle for input file
	SNDFILE	*infile;
	
	//has info on file loaded
	SF_INFO sfinfo;
	
	int bit_size;
};

#endif
