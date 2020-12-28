#ifndef REVERB_ZONE_H
#define REVERB_ZONE_H

#include "effect-zone.h"

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

struct EAXReverbZoneSaveData
{
	//name
	std::string name;
	
	//position
	double x;
	double y;
	double z;
	
	//size
	double width;
	
	//properties
	ReverbEAXProperties properties;
	
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

struct StandardReverbZoneSaveData
{
	//name
	std::string name;
	
	//position
	double x;
	double y;
	double z;
	
	//size
	double width;
	
	//properties
	ReverbStandardProperties properties;
};

class ReverbZone : public EffectZone
{
public:
	ReverbZone();
	~ReverbZone();
	
	//type of reverb to use for zone
	enum class Type: std::int8_t {NONE=0, STANDARD, EAX};
	
	void SetType(ReverbZone::Type& type);
	ReverbZone::Type& GetType();
	
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
	
	
	virtual ALuint* GetEffectPointer();
	virtual ALuint* GetEffectsSlotPointer();
	
	virtual ALuint GetEffect();
	virtual ALuint GetEffectsSlot();
	
	virtual void FreeEffects();
	
	StandardReverbZoneSaveData GetStandardReverbZoneSaveData();
	void LoadStandardReverbZoneSaveData(StandardReverbZoneSaveData& data);
	
	EAXReverbZoneSaveData GetEAXReverbZoneSaveData();
	void LoadEAXReverbZoneSaveData(EAXReverbZoneSaveData& data);
	
	//changed from effect zone to implement save data
	void SetPositionX(double& x); 
	void SetPositionY(double& y); 
	void SetPositionZ(double& z); 
	void ChangeWidth(double width);
	
private:

	ALuint m_effect;
	ALuint m_slot;
	
	//type of reverb zone
	ReverbZone::Type m_type;
	
	ALuint LoadStandardReverbEffect(const EFXEAXREVERBPROPERTIES *reverb);
	ALuint LoadEAXReverbEffect(const EFXEAXREVERBPROPERTIES *reverb);
	
	ReverbStandardProperties m_standard_prop;
	ReverbEAXProperties m_eax_prop;
	
	ZoneColor standardColor;
	ZoneColor eaxColor;
	
	StandardReverbZoneSaveData m_saveDataStandard;
	EAXReverbZoneSaveData m_saveDataEAX;
};

#endif
