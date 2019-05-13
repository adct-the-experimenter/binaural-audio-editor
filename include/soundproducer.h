#ifndef SOUNDPRODUCER_H
#define SOUNDPRODUCER_H

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft

#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

#include <osg/ShapeDrawable> //for object to render on screen
#include <osg/PositionAttitudeTransform> //for matrix transform that moves object rendered
#include <osg/Geode> //for object rendered to be moved on screen by matrix transform

//This is a class that holds positional info on object
//as well as source and buffer components for use with OpenALSoftAudioEngine

//DOES NOT PLAY SOUND. OpenALSoftAudioEngine plays the sound.
//It only holds information to pass to OpenALSoftAudioEngine to play sound.

class SoundProducer
{
	public:
		SoundProducer();
		~SoundProducer();
		
		void SetNameString(std::string& thisName);
		std::string GetNameString();
		
		void setPositionX(double& x); //set x position of sound producer
		double getPositionX(); //get x position of sound producer
		void setPositionY(double& y); //set y position of sound producer
		double getPositionY(); //get y position of sound producer
		void setPositionZ(double& z); //set z position of sound producer
		double getPositionZ(); //get z position of sound producer
		
		void InitSoundProducer(std::string& thisName, std::string& filepath, ALuint& buffer,
								double& x, double& y, double& z);
		
		void setFilepathToSound(std::string& filepath);
		std::string& getFilepathToSound();
		
		void setBuffer(ALuint& thisSource);
		ALuint* getBuffer();
		
		void CreateSourceFromBuffer(); //function to use openal soft audio engine to create source from buffer
		
		void setSource(ALuint& thisBuffer);
		ALuint* getSource();
		
		osg::ShapeDrawable* getRenderObject();
		
		osg::Geode* getGeodeNode();
		
		osg::PositionAttitudeTransform* getTransformNode();
		
	private:
		//Name of Sound Producer
		std::string name;
		
		//position of Sound Producer
		std::vector <double> producer_position_vector;
		enum POSITION_INDEX { X=0,Y=1,Z=2 };
		
		//file path to sound made
		std::string m_filepath;
		
		//buffer to play
		ALuint m_buffer;
    
		//source to play buffer
		ALuint m_source;
		
		void moveSource(); //function to move source to producer position vector coordinates
		
		//ShapeDrawable object to render
		osg::ref_ptr<osg::ShapeDrawable> m_renderObject;
		
		osg::ref_ptr<osg::Box> m_box; 
		
		//holds geometry information for rendering, moved by transform of matrix
		osg::ref_ptr<osg::Geode> m_geode;
		
		//moves the geode
		osg::ref_ptr<osg::PositionAttitudeTransform> m_paTransform;
};

#endif
