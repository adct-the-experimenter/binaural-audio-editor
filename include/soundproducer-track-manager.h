#ifndef SOUNDPRODUCER_TRACK_MANAGER_H
#define SOUNDPRODUCER_TRACK_MANAGER_H

#include <wx/wx.h>

#include "soundproducer-track.h"

#include "soundproducer-registry.h"

//class to manipulate x,y z position of sound producer
class SoundProducerTrackManager : public Track
{
public:
	SoundProducerTrackManager(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext);
	~SoundProducerTrackManager();
	
	//functions to set reference to audio device and audio context to use for player
	void SetReferenceToAudioDevice(ALCdevice* thisAudioDevice);
	void SetReferenceToAudioContext(ALCcontext* thisAudioContext);
	
	void AddNewSoundProducerTrackToManager(SoundProducerTrack* thisTrack);
	void RemoveLastSoundProducerTrackFromManager();
	
	//Functions inherited from Track
	void SetReferenceToCurrentTimeVariable(double* thisTimeVariable);
	double GetCurrentTime();
	
	
	
	//function to call in timer loop, variable to manipulate gets changed here
	virtual void FunctionToCallInPlayState();
    virtual void FunctionToCallInPauseState();
    virtual void FunctionToCallInRewindState();
    virtual void FunctionToCallInFastForwardState();
    virtual void FunctionToCallInNullState();
	
private:
	
	//vector to contain pointers to sound producer tracks
	std::vector <SoundProducerTrack*> soundProducerTracks_vec;
	
	//vector to contain pointers to sound producer track sources
	std::vector <ALuint*> soundproducertracks_sources_vector;
	
	OpenALSoftPlayer* audioPlayer;
	
	ALCdevice* audioDevicePtr; //pointer to audio device to be used
    ALCcontext* alContextPtr; //pointer to context of where audio is played

};

#endif
