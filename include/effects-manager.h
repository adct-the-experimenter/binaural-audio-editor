#ifndef EFFECTS_MANAGER_H
#define EFFECTS_MANAGER_H

#include "soundproducer-track-manager.h"
#include "reverb-zone.h"

#include <wx/wx.h>


//class to manipulate x,y z position of sound producer
class EffectsManager
{
public:
	EffectsManager(SoundProducerTrackManager* track_manager);
	~EffectsManager();
	
	//function to create reverb zone that uses standard effects
	void CreateStandardReverbZone(std::string& name, double& x, double& y, double& z, double& width, ReverbStandardProperties& properties);
	
	//function to create reverb zone that uses EAX effectss
	void CreateEAXReverbZone(std::string& name, double& x, double& y, double& z, double& width, ReverbEAXProperties& properties);
	
	//function to change attributes of reverb zone
	void ChangeReverbZoneAttributes();
	
private:
	
	//pointer to manager that contains all soundproducer tracks used
	SoundProducerTrackManager* m_track_manager_ptr;
	
	//vector to contain many reverb zone objects
	std::vector <ReverbZone> reverb_zones_vector;
	

};


#endif
