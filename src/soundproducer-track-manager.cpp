#include "soundproducer-track-manager.h"

SoundProducerTrackManager::SoundProducerTrackManager(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext) : Track(title)
{
	//initialize audio player
	audioPlayer = new OpenALSoftPlayer();
	audioPlayer->SetReferenceToAudioContext(thisAudioContext);
	audioPlayer->SetReferenceToAudioDevice(thisAudioDevice);
}

SoundProducerTrackManager::~SoundProducerTrackManager()
{
	if(audioPlayer != nullptr)
	{
		delete audioPlayer;
	}
}
	
void SoundProducerTrackManager::SetReferenceToAudioDevice(ALCdevice* thisAudioDevice){audioDevicePtr = thisAudioDevice;}
void SoundProducerTrackManager::SetReferenceToAudioContext(ALCcontext* thisAudioContext){alContextPtr = thisAudioContext;}

void SoundProducerTrackManager::AddNewSoundProducerTrackToManager(SoundProducerTrack* thisTrack)
{
	soundProducerTracks_vec.push_back(thisTrack);
	soundproducertracks_sources_vector.push_back(thisTrack->GetReferenceToTrackSource());
}

void SoundProducerTrackManager::RemoveLastSoundProducerTrackFromManager()
{
	soundProducerTracks_vec.pop_back();
	soundproducertracks_sources_vector.pop_back();
}

//Functions inherited from Track



void SoundProducerTrackManager::SetReferenceToCurrentTimeVariable(double* thisTimeVariable){Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);}

double SoundProducerTrackManager::GetCurrentTime(){return Track::GetCurrentTime();}



//function to call in timer loop, variable to manipulate gets changed here
void SoundProducerTrackManager::FunctionToCallInPlayState()
{
	//buffer audio for all track sources
	for(size_t i=0; i < soundProducerTracks_vec.size(); i++)
	{
		soundProducerTracks_vec[i]->FunctionToCallInPlayState();
	}
	//play all sources in sync
	audioPlayer->PlayMultipleUpdatedPlayerBuffers(&soundproducertracks_sources_vector);
}

void SoundProducerTrackManager::FunctionToCallInPauseState()
{
	for(size_t i=0; i < soundProducerTracks_vec.size(); i++)
	{
		soundProducerTracks_vec[i]->FunctionToCallInPauseState();
	}
}

void SoundProducerTrackManager::FunctionToCallInRewindState()
{
	for(size_t i=0; i < soundProducerTracks_vec.size(); i++)
	{
		soundProducerTracks_vec[i]->FunctionToCallInRewindState();
	}
}

void SoundProducerTrackManager::FunctionToCallInFastForwardState()
{
	for(size_t i=0; i < soundProducerTracks_vec.size(); i++)
	{
		soundProducerTracks_vec[i]->FunctionToCallInFastForwardState();
	}
}

void SoundProducerTrackManager::FunctionToCallInNullState()
{
	for(size_t i=0; i < soundProducerTracks_vec.size(); i++)
	{
		soundProducerTracks_vec[i]->FunctionToCallInNullState();
	}
}
