#include "effects-manager.h"

EffectsManager::EffectsManager(SoundProducerTrackManager* track_manager)
{
	m_track_manager_ptr = track_manager;
}

EffectsManager::~EffectsManager()
{
	m_track_manager_ptr = nullptr;
}


void EffectsManager::CreateStandardReverbZone(std::string& name, double& x, double& y, double& z, double& width, ReverbStandardProperties& properties)
{
	ReverbZone r_zone;
	
	reverb_zones_vector.push_back(r_zone);
	
	reverb_zones_vector.back().InitStandardReverbZone(name,x,y,z,width,properties);
}


void EffectsManager::CreateEAXReverbZone(std::string& name, double& x, double& y, double& z, double& width, ReverbEAXProperties& properties)
{
	ReverbZone r_zone;
	
	reverb_zones_vector.push_back(r_zone);
	
	reverb_zones_vector.back().InitEAXReverbZone(name,x,y,z,width,properties);
}

std::vector <ReverbZone> *EffectsManager::GetReferenceToReverbZoneVector(){return &reverb_zones_vector;}
