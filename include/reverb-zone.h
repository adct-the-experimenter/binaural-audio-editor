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

struct ReverbEAXProperties
{
	//AL_EAXREVERB_DENSITY, 
	double flDensity;
	//AL_EAXREVERB_DIFFUSION, 
	double flDiffusion;
	//AL_EAXREVERB_GAIN, 
	double flGain;
	//AL_EAXREVERB_GAINHF, 
	double flGainHF;
	//AL_EAXREVERB_GAINLF, 
	double flGainLF;
	//AL_EAXREVERB_DECAY_TIME, 
	double flDecayTime;
	//AL_EAXREVERB_DECAY_HFRATIO, 
	double flDecayHFRatio;
	//AL_EAXREVERB_DECAY_LFRATIO, 
	double flDecayLFRatio;
	//AL_EAXREVERB_REFLECTIONS_GAIN, 
	double flReflectionsGain;
	//AL_EAXREVERB_REFLECTIONS_DELAY, 
	double flReflectionsDelay;
	//alEffectfv(effect, AL_EAXREVERB_REFLECTIONS_PAN, double flReflectionsPan;
	//AL_EAXREVERB_LATE_REVERB_GAIN, 
	double flLateReverbGain;
	//AL_EAXREVERB_LATE_REVERB_DELAY, 
	double flLateReverbDelay;
	//alEffectfv(effect, AL_EAXREVERB_LATE_REVERB_PAN, double flLateReverbPan;
	//AL_EAXREVERB_ECHO_TIME, 
	double flEchoTime;
	//AL_EAXREVERB_ECHO_DEPTH, 
	double flEchoDepth;
	//AL_EAXREVERB_MODULATION_TIME, 
	double flModulationTime;
	//AL_EAXREVERB_MODULATION_DEPTH, 
	double flModulationDepth;
	//AL_EAXREVERB_HFREFERENCE, 
	double flHFReference;
	//AL_EAXREVERB_LFREFERENCE, 
	double flLFReference;
	//AL_EAXREVERB_AIR_ABSORPTION_GAINHF, 
	double flAirAbsorptionGainHF;
	//AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, 
	double flRoomRolloffFactor;
	//AL_EAXREVERB_DECAY_HFLIMIT, 
	//int iDecayHFLimit; 
};

struct ReverbStandardProperties
{
	//AL_REVERB_DENSITY, 
	double flDensity;
	//AL_REVERB_DIFFUSION, 
	double flDiffusion;
	//AL_REVERB_GAIN, 
	double flGain;
	//AL_REVERB_GAINHF, 
	double flGainHF;
	//AL_REVERB_DECAY_TIME, 
	double flDecayTime;
	//AL_REVERB_DECAY_HFRATIO, 
	double flDecayHFRatio;
	//AL_REVERB_REFLECTIONS_GAIN, 
	double flReflectionsGain;
	//AL_REVERB_REFLECTIONS_DELAY, 
	double flReflectionsDelay;
	//AL_REVERB_LATE_REVERB_GAIN, 
	double flLateReverbGain;
	//AL_REVERB_LATE_REVERB_DELAY, 
	double flLateReverbDelay;
	//AL_REVERB_AIR_ABSORPTION_GAINHF, 
	double flAirAbsorptionGainHF;
	//AL_REVERB_ROOM_ROLLOFF_FACTOR, 
	double flRoomRolloffFactor;
	//AL_REVERB_DECAY_HFLIMIT, 
	//int iDecayHFLimit;
};

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
	
	
	//functions to initialize reverb zones based on type
	
	void InitStandardReverbZone(std::string& thisName,
							double& x, double& y, double& z, double& width,
							ReverbStandardProperties& properties);
							
	void InitStandardReverbZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width,
							ReverbStandardProperties& properties);
							
	void InitEAXReverbZone(std::string& thisName,
							double& x, double& y, double& z, double& width,
							ReverbEAXProperties& properties);
							
	void InitEAXReverbZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width,
							ReverbEAXProperties& properties);
	
							
	//function to change reverb properties 
	void ChangeStandardReverbZoneProperties(ReverbStandardProperties& properties);
	void ChangeEAXReverbZoneProperties(ReverbEAXProperties& properties);
	
	ReverbStandardProperties& GetStandardReverbZoneProperties();
	ReverbEAXProperties& GetEAXReverbZoneProperties();
	
	//3d Object properties
	osg::ShapeDrawable* getRenderObject();

	osg::Geode* getGeodeNode();

	osg::PositionAttitudeTransform* getTransformNode();
	
	
	
	//OpenAL Soft effects properties
	
	ALuint* GetEffect();
	ALuint* GetEffectsSlot();
	
	void FreeEffects();
	
private:
    
    //effect
    ALuint m_effect; 
    
    //what play effect in source of soundproducer 
    ALuint m_slot;
    
	//Name of Reverb Zone
	std::string name;
	
	//type of reverb zone
	ReverbZone::Type m_type;

	//position of reverb zone
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
	
	ALuint LoadStandardReverbEffect(const EFXEAXREVERBPROPERTIES *reverb);
	ALuint LoadEAXReverbEffect(const EFXEAXREVERBPROPERTIES *reverb);
	
	ReverbStandardProperties m_standard_prop;
	ReverbEAXProperties m_eax_prop;
};

#endif
