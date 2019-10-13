#ifndef REVERB_ZONE_H
#define REVERB_ZONE_H

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft
#include "AL/efx.h"
#include "AL/efx-presets.h"

#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

#include <osg/ShapeDrawable> //for object to render on screen
#include <osg/PositionAttitudeTransform> //for matrix transform that moves object rendered
#include <osg/Geode> //for object rendered to be moved on screen by matrix transform



class ReverbZone
{
public:
	ReverbZone();
	~ReverbZone();
	
	//name of reverb zone
	
	void SetNameString(std::string& thisName);
	std::string GetNameString();
	
	//type of reverb to use for zone
	enum class Type: std::int8_t {NONE=0, STANDARD, EAX};
	
	void SetType(ReverbZone::Type& type);
	ReverbZone::Type& GetType();
	
	//Position
	
	void SetPositionX(double& x); //set x position of sound producer
	double GetPositionX(); //get x position of sound producer
	void SetPositionY(double& y); //set y position of sound producer
	double GetPositionY(); //get y position of sound producer
	void SetPositionZ(double& z); //set z position of sound producer
	double GetPositionZ(); //get z position of sound producer
	
	
	//functions to init reverb zones based on type
	
	void InitStandardReverbZone(std::string& thisName,
							double& x, double& y, double& z, double& width);
							
	void InitEAXReverbZone(std::string& thisName,
							double& x, double& y, double& z, double& width);
	//3d Object properties
	osg::ShapeDrawable* getRenderObject();

	osg::Geode* getGeodeNode();

	osg::PositionAttitudeTransform* getTransformNode();
	
	
	
	//OpenAL Soft effects properties
	
	ALuint* GetEffect();
	ALuint* GetEffectsSlot();
	
private:
	
	//reverb property
	EFXEAXREVERBPROPERTIES reverb;
    
    //effect
    ALuint m_effect; 
    
    //what play effect in source of soundproducer 
    ALuint m_slot;
    
	//Name of Reverb Zone
	std::string name;
	
	//type of reverb zone
	ReverbZone::Type m_type;

	//position of reverb zone
	std::vector <double> producer_position_vector;
	enum POSITION_INDEX { X=0,Y=1,Z=2 };
	
	double m_width;
	
	//ShapeDrawable object to render
	osg::ref_ptr<osg::ShapeDrawable> m_renderObject;

	osg::ref_ptr<osg::Box> m_box;

	//holds geometry information for rendering, moved by transform of matrix
	osg::ref_ptr<osg::Geode> m_geode;

	//moves the geode
	osg::ref_ptr<osg::PositionAttitudeTransform> m_paTransform;

};

#endif
