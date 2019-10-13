#ifndef REVERB_ZONE_H
#define REVERB_ZONE_H

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

class ReverbZone
{
	public:
		ReverbZone();
		~ReverbZone();

		void SetNameString(std::string& thisName);
		std::string GetNameString();

		void SetPositionX(double& x); //set x position of sound producer
		double GetPositionX(); //get x position of sound producer
		void SetPositionY(double& y); //set y position of sound producer
		double GetPositionY(); //get y position of sound producer
		void SetPositionZ(double& z); //set z position of sound producer
		double GetPositionZ(); //get z position of sound producer

		void InitReverbZone(std::string& thisName,
								double& x, double& y, double& z, double& width, double& height);

		osg::ShapeDrawable* getRenderObject();

		osg::Geode* getGeodeNode();

		osg::PositionAttitudeTransform* getTransformNode();

	private:
		//Name of Sound Producer
		std::string name;

		//position of Sound Producer
		std::vector <double> producer_position_vector;
		enum POSITION_INDEX { X=0,Y=1,Z=2 };

		
		//ShapeDrawable object to render
		osg::ref_ptr<osg::ShapeDrawable> m_renderObject;

		osg::ref_ptr<osg::Box> m_box;

		//holds geometry information for rendering, moved by transform of matrix
		osg::ref_ptr<osg::Geode> m_geode;

		//moves the geode
		osg::ref_ptr<osg::PositionAttitudeTransform> m_paTransform;
		
		void moveZone();
};

#endif
