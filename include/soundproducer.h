#ifndef SOUNDPRODUCER_H
#define SOUNDPRODUCER_H

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft


#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

//This is a class that hold positional info on object
//as well as source and buffer components for use with OpenALSoftAudioEngine

//DOES NOT PLAY SOUND. OpenALSoftAudioEngine plays the sound.
//It only holds information to pass to OpenALSoftAudioEngine to play sound.

class SoundProducer
{
	public:
		SoundProducer();
		~SoundProducer();
		
		void setListenerPositionX(float x); //set x position of sound producer
		float getListenerPositionX(); //get x position of sound producer
		void setListenerPositionY(float y); //set y position of sound producer
		float getListenerPositionY(); //get y position of sound producer
		void setListenerPositionZ(float z); //set z position of sound producer
		float getListenerPositionZ(); //get z position of sound producer
		
		void setFilepathToSound(std::string& filepath);
		std::string& getFilepathToSound();
		
		ALuint* getBuffer();
		ALuint* getSource();
		
	private:
		//position of Listener
		std::vector <float> producer_position_vector;
		enum POSITION_INDEX { X=0,Y=1,Z=2 };
		
		//file path to sound made
		std::string m_filepath;
		
		//buffer to play
		ALuint m_buffer;
    
		//source to play buffer
		ALuint m_source;
		
};
#endif
