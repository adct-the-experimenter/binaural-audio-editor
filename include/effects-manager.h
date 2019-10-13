#ifndef EFFECTS_MANAGER_H
#define EFFECTS_MANAGER_H

#include "soundproducer-track-manager.h"

#include <wx/wx.h>


//class to manipulate x,y z position of sound producer
class EffectsManager
{
public:
	EffectsManager(SoundProducerTrackManager* track_manager);
	~EffectsManager();
	
private:
	
	//pointer to manager that contains all soundproducer tracks used
	SoundProducerTrackManager* m_track_manager_ptr;

};


#endif
