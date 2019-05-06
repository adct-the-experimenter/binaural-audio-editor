#ifndef OPENALSOFTAUDIOENGINE_H
#define OPENALSOFTAUDIOENGINE_H

#include <assert.h>
#include <math.h>

#include "time.h"

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft


#include "sndfile.h"

#include <cstring> //for memset
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

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

//Listener Position Functions

    void setListenerPositionX(float x); //set x position of listener
    float getListenerPositionX(); //get x position of listener
    void setListenerPositionY(float y); //set y position of listener
    float getListenerPositionY(); //get y position of listener
    void setListenerPositionZ(float z); //set z position of listener
    float getListenerPositionZ(); //get z position of listener

//Listener Orientation Functions
    void setListenerForwardX(float x); //set x of forward of listener
    float getListenerForwardX(); //get x of forward of listener
    void setListenerForwardY(float y); //set y of forward of listener
    float getListenerForwardY(); //get y of forward of listener
    void setListenerForwardZ(float z); //set z of forward of listener
    float getListenerForwardZ(); //get z of forward listener

    void setListenerUpX(float x); //set x of up of listener
    float getListenerUpX(); //get x of up of listener
    void setListenerUpY(float y); //set y of up of listener
    float getListenerUpY(); //get y of up of listener
    void setListenerUpZ(float z); //set z of up of listener
    float getListenerUpZ(); //get z of up of listener
    
//HRTF
    //function to perform tests for HRTF support
    void testHRTF();
    std::string getHRTFTestResult();
    void clear_testHRTFResults();

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

    //position of Listener
    std::vector <float> listener_position_vector;
    enum POSITION_INDEX { X=0,Y=1,Z=2 };
	void setListenerPosition(); //function to set listener position based on listener position vector coordinates
	
    //orientation of Listener
    std::vector<float> listener_orientation_vector; //vector to hold values of listener orientation
    //first 3 values are forward vector xyz , last 3 values are up vector xyz
    //enum to help set orientation vector
    enum ORIENTATION_INDEX { FORWARD_X=0,FORWARD_Y=1,FORWARD_Z=2,
                                                 UP_X=3, UP_Y=4, UP_Z=5 };
	void setListenerOrientation(); //function to set listener orientation based on listener orientation vector coordinates

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
