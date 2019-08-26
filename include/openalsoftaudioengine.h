#ifndef OPENALSOFTAUDIOENGINE_H
#define OPENALSOFTAUDIOENGINE_H

#include <assert.h>
#include <math.h>

#include "time.h"

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft


#include "sndfile.h"

#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

#include "listener.h"

/*	This will be the length of the buffer used to hold frames while
**	we process them.
*/
#define		BUFFER_LEN	1024

/* libsndfile can handle more than 6 channels but we'll restrict it to 2. */
#define		MAX_CHANNELS	2


//class inherits from QQMLPropertValue
class OpenAlSoftAudioEngine 
{
	
    
public:

    OpenAlSoftAudioEngine();
    ~OpenAlSoftAudioEngine();

    //function to initialize openAl Soft
    
    bool initOpenALSoft();

    //function to clean up openAL Soft initialization
    void close_openALSoft();

	ALCdevice* GetReferenceToAudioDevice();
	ALCcontext* GetReferenceToAudioContext();
	
//HRTF
    //functions to perform tests for HRTF support
    void TestHRTF();
    std::string getHRTFTestResult();
    void clear_testHRTFResults();
    
    void GetAvailableHRTFNames(std::vector <std::string> *names_vector);
	std::string GetCurrentHRTFSelected();
	
	void SelectThisHRTFByIndex(int& index,std::string& message);
	
//Sound Playback Related Functions

	//loading buffer
	void loadSound(ALuint* buffer,const std::string& filename); //function to take in file path to sound file and load buffer info to ALuint buffer
	std::string getLoadSoundResult();//function to return load sound results string
	void clear_LoadSoundResults();

	//Creating source to play sound
	void createSource(ALuint* buffer,ALuint* source);	
	
	void playSound(ALuint* source);
	
private:

    //OpenAL Soft sound setup variables
    ALCdevice* gAudioDevice; //audio device to be used
    ALCcontext* alContext; //context of where audio is played

	
//********************************************************
//******************** Testing ***************************
//********************************************************

    //error flag variable to test if there is error anywhere regarding OpenAL Soft.
    ALenum test_error_flag;
    void error_check(std::string location_str);
    
    std::string loadSound_Results; //string variable to write results of loadSound to
    std::string testHRTF_Results; //string variable to write results of HRTF test to

};

#endif // OPENALSOFTAUDIOENGINE_H
