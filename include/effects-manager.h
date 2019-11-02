#ifndef EFFECTS_MANAGER_H
#define EFFECTS_MANAGER_H

#include "listener.h"
#include "soundproducer-track-manager.h"
#include "reverb-zone.h"

#include <wx/wx.h>




//class to manipulate x,y z position of sound producer
class EffectsManager
{
public:
	EffectsManager(SoundProducerTrackManager* track_manager, Listener* listener);
	~EffectsManager();
	
	//function to create reverb zone that uses standard effects
	void CreateStandardReverbZone(std::string& name, double& x, double& y, double& z, double& width, ReverbStandardProperties& properties);
	
	//function to create reverb zone that uses EAX effectss
	void CreateEAXReverbZone(std::string& name, double& x, double& y, double& z, double& width, ReverbEAXProperties& properties);
	
	//function to return a pointer to reverb zone vector
	std::vector <ReverbZone> *GetReferenceToReverbZoneVector();
	
	//function to return a pointer to reverb zone from index in vector
	ReverbZone* GetPointerToReverbZone(size_t& index);
	
	//function to run to apply reverb zone effect if listener is in reverb zone
	void RunListenerInReverbZoneOperation();
	
	friend class CheckListenerReverbZoneThread;
	
private:
	
	//pointer to manager that contains all soundproducer tracks used
	SoundProducerTrackManager* m_track_manager_ptr;
	
	//pointer to listener
	Listener* m_listener_ptr;
	
	//vector to contain many reverb zone objects
	std::vector <ReverbZone> reverb_zones_vector;
	
	//function to perform the entire reverb thread operation of checking and setting reverb
	void PerformReverbThreadOperation();
	
	//function to return bool of if a listener is in a reverb zone
	bool IsListenerInThisReverbZone(ReverbZone* thisZone);
	
	//function to return bool of if a sound producer is in a reverb zone
	bool IsThisSoundProducerInsideReverbZone(SoundProducer* thisSoundProducer,ReverbZone* thisZone);
	
	//function to apply reverb effect of a zone to sound producer track
	void ApplyThisReverbZoneEffectToThisTrack(SoundProducerTrack* thisSoundProducerTrack, ReverbZone* thisZone);
	
	//function to remove effect applied to the sound producer track
	void RemoveEffectFromThisTrack(SoundProducerTrack* thisSoundProducerTrack);
};



#endif
