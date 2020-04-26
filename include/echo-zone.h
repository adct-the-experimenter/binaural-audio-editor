#ifndef ECHO_ZONE_H
#define ECHO_ZONE_H

#include "effect-zone.h"

struct EchoZoneProperties
{
	//AL_ECHO_DELAY
	double flDelay;
	//AL_ECHO_LRDELAY
	double flLRDelay;
	//AL_ECHO_DAMPING
	double flDamping;
	//AL_ECHO_FEEDBACK
	double flFeedback;
	//AL_ECHO_SPREAD
	double flSpread;
};

class EchoZone : public EffectZone
{
public:
	EchoZone();
	~EchoZone();
	
	
	//functions to initialize echo zones
	
	void InitEchoZone(std::string& thisName,
							double& x, double& y, double& z, double& width,
							EchoZoneProperties& properties);
							
	void InitEchoZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width,
							EchoZoneProperties& properties);
							
	
							
	//function to change reverb properties 
	void ChangeEchoZoneProperties(EchoZoneProperties& properties);
	
	EchoZoneProperties& GetEchoZoneProperties();
	
	
private:
	
	ALuint LoadStandardReverbEffect(const EFXEAXREVERBPROPERTIES *reverb);
	ALuint LoadEAXReverbEffect(const EFXEAXREVERBPROPERTIES *reverb);
	
	EchoZoneProperties m_echo_prop;
	
	ZoneColor echoZoneColor;
};

#endif
