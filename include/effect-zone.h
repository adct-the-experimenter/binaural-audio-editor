#ifndef EFFECT_ZONE_H
#define EFFECT_ZONE_H


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

struct ZoneColor
{
	float r; //red
	float g; //green
	float b; //blue
	float alpha; //transparency
};

class EffectZone
{

public:
	EffectZone();
	~EffectZone();
	
	//name of zone
	
	void SetNameString(std::string& thisName);
	std::string GetNameString();
	
	
	//Position
	
	void SetPositionX(double& x); //set x position of sound producer
	double GetPositionX(); //get x position of sound producer
	void SetPositionY(double& y); //set y position of sound producer
	double GetPositionY(); //get y position of sound producer
	void SetPositionZ(double& z); //set z position of sound producer
	double GetPositionZ(); //get z position of sound producer
	
	
	//functions to initialize effect zone
	
	void InitEffectZone(std::string& thisName,
							double& x, double& y, double& z, double& width);
							
	void InitEffectZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width,
							ZoneColor& color);
	
	
	//3d Object properties
	osg::ShapeDrawable* getRenderObject();

	osg::Geode* getGeodeNode();

	osg::PositionAttitudeTransform* getTransformNode();
	
	void ChangeWidth(double width);
	double GetWidth();
	
	void SetColor(ZoneColor color);
	
	//OpenAL Soft effects properties
	
	void SetEffectsSlotPointer(ALuint* slot_ptr);
	
	virtual ALuint* GetEffectPointer();
	virtual ALuint* GetEffectsSlotPointer();
	
	virtual ALuint GetEffect();
	virtual ALuint GetEffectsSlot();
	
	virtual void FreeEffects();
	

private:

	ALuint* m_slot_ptr;
    
	//Name of echo Zone
	std::string name;
	
	//position of echo zone
	std::vector <double> position_vector;
	enum POSITION_INDEX { X=0,Y=1,Z=2 };
	
	double m_width;
	
	//ShapeDrawable object to render
	osg::ref_ptr<osg::ShapeDrawable> m_renderObject;

	osg::ref_ptr<osg::Box> m_box;

	//holds geometry information for rendering, moved by transform of matrix
	osg::ref_ptr<osg::Geode> m_geode;

	//moves the geode
	osg::ref_ptr<osg::PositionAttitudeTransform> m_paTransform;
	
	ZoneColor m_color;
};

#endif
