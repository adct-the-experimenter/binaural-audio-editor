#include "effects-manager.h"

EffectsManager::EffectsManager(SoundProducerTrackManager* track_manager)
{
	m_track_manager_ptr = track_manager;
}

EffectsManager::~EffectsManager()
{
	m_track_manager_ptr = nullptr;
}
