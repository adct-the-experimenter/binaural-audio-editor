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

ReverbZone* EffectsManager::GetPointerToReverbZone(size_t& index){return &reverb_zones_vector[index];}

bool EffectsManager::IsListenerInThisReverbZone(ReverbZone* thisZone)
{
	osg::BoundingBox zone_box = thisZone->getRenderObject()->computeBoundingBox();
	osg::BoundingBox listener_box = m_listener_ptr->getRenderObject()->computeBoundingBox();
	
	//if bounding box of listener intersects bounding box of reverb zone
	if(zone_box.intersects(listener_box) )
	{
		return true;
	}
	
	return false;
}

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

	long endtime = ::wxGetLocalTime()+1;
	while (!TestDestroy() )
	{
		wxMilliSleep(500); //sleep for 500 milliseconds
		
		//if sound is being played and there are reverb zones
		if(m_effects_manager_ptr->m_track_manager_ptr->IsSoundBeingPlayed() && m_effects_manager_ptr->GetReferenceToReverbZoneVector()->size() > 0)
		{
			for(size_t i = 0; i < m_effects_manager_ptr->GetReferenceToReverbZoneVector()->size(); i++)
			{
				//check if listener is in reverb zone
				ReverbZone* thisZone = m_effects_manager_ptr->GetPointerToReverbZone(i);
				
				//check if zone is initialized
				if(thisZone->GetType() != ReverbZone::Type::NONE && thisZone->getGeodeNode() != nullptr)
				{
					if(m_effects_manager_ptr->IsListenerInThisReverbZone(thisZone))
					{
						//if listener is in the reverb zone
						std::cout << "Listener is in the reverb zone!\n";
					}
				}
			}
		}
		
	}
	
	return nullptr;  
}
