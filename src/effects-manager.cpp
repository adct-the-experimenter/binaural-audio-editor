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
	
	standard_reverb_zones_vector.push_back(r_zone);
	
	standard_reverb_zones_vector.back().InitStandardReverbZoneWithGraphicalObject(name,x,y,z,width,properties);
	
	effect_zones_vector.push_back(&standard_reverb_zones_vector.back());
	
}


void EffectsManager::CreateEAXReverbZone(std::string& name, double& x, double& y, double& z, double& width, ReverbEAXProperties& properties)
{
	ReverbZone r_zone;
	
	eax_reverb_zones_vector.push_back(r_zone);
	
	eax_reverb_zones_vector.back().InitEAXReverbZoneWithGraphicalObject(name,x,y,z,width,properties);
	
	effect_zones_vector.push_back(&eax_reverb_zones_vector.back());
	
}

void EffectsManager::CreateEchoZone(std::string& name, double& x, double& y, double& z, double& width, EchoZoneProperties& properties)
{
	EchoZone e_zone;
	
	echo_zones_vector.push_back(e_zone);
	
	echo_zones_vector.back().InitEchoZoneWithGraphicalObject(name,x,y,z,width,properties);
	
	effect_zones_vector.push_back(&echo_zones_vector.back());
	
}

std::vector <EffectZone*> *EffectsManager::GetReferenceToEffectZoneVector(){return &effect_zones_vector;}

EffectZone* EffectsManager::GetPointerToEffectZone(size_t& index){return effect_zones_vector[index];}

ReverbZone* EffectsManager::GetPointerToStandardReverbZone(size_t& index){return &standard_reverb_zones_vector[index];}
ReverbZone* EffectsManager::GetPointerToEAXReverbZone(size_t& index){return &eax_reverb_zones_vector[index];}

EchoZone* EffectsManager::GetPointerToEchoZone(size_t& index){return &echo_zones_vector[index];}

void EffectsManager::PerformReverbThreadOperation()
{
	//if sound is being played and there are reverb zones
	if(m_track_manager_ptr->IsSoundBeingPlayed() && effect_zones_vector.size() > 0)
	{
		for(size_t i = 0; i < effect_zones_vector.size(); i++)
		{
			//check if listener is in reverb zone
			EffectZone* thisZone = effect_zones_vector[i];
			
			//check if zone is initialized
			if( thisZone->getTransformNode() != nullptr)
			{
				if(EffectsManager::IsListenerInThisEffectZone(thisZone))
				{
					//if listener is in the reverb zone
					
					//check if sound producers are inside the zone
					if(m_track_manager_ptr->soundProducerTracks_vec->size() > 0)
					{
						for(size_t i = 0; i < m_track_manager_ptr->soundProducerTracks_vec->size(); i++)
						{
							SoundProducerTrack* thisSoundProducerTrack = m_track_manager_ptr->soundProducerTracks_vec->at(i);
							
							//if no reverb is applied
							if(!thisSoundProducerTrack->IsReverbApplied())
							{
								SoundProducer* thisSoundProducer = thisSoundProducerTrack->GetReferenceToSoundProducerManipulated();
							
								//if there is a sound producer attached to sound producer track
								if(thisSoundProducer != nullptr)
								{
									//if sound producer is inside the zone
									if(EffectsManager::IsThisSoundProducerInsideEffectZone(thisSoundProducer,thisZone))
									{
										//std::cout << "SoundProducer is inside the reverb zone!\n";
										//apply reverb to source of sound producer track
										EffectsManager::ApplyThisEffectZoneEffectToThisTrack(thisSoundProducerTrack,thisZone);
									}
								}
							}
							
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					//if listener is not in the reverb zone
					
					//check if sound producers are inside the zone
					if(m_track_manager_ptr->soundProducerTracks_vec->size() > 0)
					{
						for(size_t i = 0; i < m_track_manager_ptr->soundProducerTracks_vec->size(); i++)
						{
							SoundProducerTrack* thisSoundProducerTrack = m_track_manager_ptr->soundProducerTracks_vec->at(i);
							
							SoundProducer* thisSoundProducer = thisSoundProducerTrack->GetReferenceToSoundProducerManipulated();
							
							//if there is a sound producer attached to sound producer track
							if(thisSoundProducer != nullptr)
							{
								//if sound producer is inside the zone
								if(EffectsManager::IsThisSoundProducerInsideEffectZone(thisSoundProducer,thisZone)
								   || thisSoundProducerTrack->IsReverbApplied())
								{
									//remove reverb effect from sound producer track
									EffectsManager::RemoveEffectFromThisTrack(thisSoundProducerTrack);
								}
							}
						}
					}
					
				}
				
			}
			
		}
		
	}
}

bool EffectsManager::IsListenerInThisEffectZone(EffectZone* thisZone)
{
	osg::BoundingSphere zone_box = thisZone->getTransformNode()->computeBound();
	osg::BoundingSphere listener_box = m_listener_ptr->getTransformNode()->computeBound();
	
	//if bounding box of listener intersects bounding box of reverb zone
	if(zone_box.intersects(listener_box) )
	{
		return true;
	}
	
	return false;
}

bool EffectsManager::IsThisSoundProducerInsideEffectZone(SoundProducer* thisSoundProducer,EffectZone* thisZone)
{
	osg::BoundingSphere zone_box = thisZone->getTransformNode()->computeBound();
	osg::BoundingSphere sound_producer_box = thisZone->getTransformNode()->computeBound();
	
	//if bounding box of listener intersects bounding box of reverb zone
	if(zone_box.intersects(sound_producer_box) )
	{
		return true;
	}
	
	return false;
}

void EffectsManager::ApplyThisEffectZoneEffectToThisTrack(SoundProducerTrack* thisSoundProducerTrack, EffectZone* thisZone)
{
	
	/* Connect the source to the effect slot. This tells the source to use the
	 * effect slot 'slot', on send #0 with the AL_FILTER_NULL filter object.
	 */
	
	ALuint* thisSource = thisSoundProducerTrack->GetReferenceToTrackSource();
	 
	alSource3i(*thisSource, AL_AUXILIARY_SEND_FILTER, (ALint)(*thisZone->GetEffectsSlotPointer()), 0, AL_FILTER_NULL);
	assert(alGetError()== AL_NO_ERROR && "Failed to setup effect for sound source send 0.");
	
	thisSoundProducerTrack->SetStatusReverbApplied(true);
}

void EffectsManager::RemoveEffectFromThisTrack(SoundProducerTrack* thisSoundProducerTrack)
{
	ALuint* thisSource = thisSoundProducerTrack->GetReferenceToTrackSource();
	
	alSource3i(*thisSource,AL_AUXILIARY_SEND_FILTER,AL_EFFECTSLOT_NULL, 0, 0); 
	assert(alGetError()==AL_NO_ERROR && "Failed to disable source send 0.");
	
	thisSoundProducerTrack->SetStatusReverbApplied(false);
}

std::vector <SoundProducerTrack*> *EffectsManager::GetReferenceToSoundProducerTracksVector()
{
	return m_track_manager_ptr->soundProducerTracks_vec;
}

void EffectsManager::FreeEffects()
{
	std::cout << "Freeing effects...\n";
	for(size_t i=0; i < standard_reverb_zones_vector.size(); i++)
	{
		standard_reverb_zones_vector[i].FreeEffects();
	}
	
	for(size_t i=0; i < eax_reverb_zones_vector.size(); i++)
	{
		eax_reverb_zones_vector[i].FreeEffects();	
	}
	
	for(size_t i=0; i < echo_zones_vector.size(); i++)
	{
		echo_zones_vector[i].FreeEffects();
	}
}

