#include "effects-manager.h"

EffectsManager::EffectsManager(SoundProducerTrackManager* track_manager, Listener* listener)
{
	m_track_manager_ptr = track_manager;
	m_listener_ptr = listener;
}

EffectsManager::~EffectsManager()
{
	m_track_manager_ptr = nullptr;
	m_listener_ptr = nullptr;
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

CheckListenerReverbZoneThread::CheckListenerReverbZoneThread(EffectsManager* manager)
{
	m_effects_manager_ptr = manager;
	
	//intialize thread outside of class
	/*
		 CheckListenerReverbZoneThread *thread = new CheckListenerReverbZoneThread(); 
		 if ( thread->Create() != wxTHREAD_NO_ERROR ) 
		 {
			wxLogError(wxT("Can't create thread!")); 
		 } 
	*/
	
	//call wxThread:: Run virtual function to start thread which runs Entry()
	
	//call wxThRead::Delete to destroy thread
}

void *CheckListenerReverbZoneThread::Entry() 
{
	while (!TestDestroy())
    {
        // ... do a bit of work...
        
        //check if listener is in reverb zone
        
    }     
	 
	
	return nullptr;  
}
