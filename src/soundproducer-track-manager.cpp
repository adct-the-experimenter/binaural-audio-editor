#include "soundproducer-track-manager.h"

SoundProducerTrackManager::SoundProducerTrackManager(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext) : Track(title)
{
	//initialize audio player
	audioPlayer = new OpenALSoftPlayer();
	audioPlayer->SetReferenceToAudioContext(thisAudioContext);
	audioPlayer->SetReferenceToAudioDevice(thisAudioDevice);
	
	soundProducerTracks_vec = nullptr;
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

void SoundProducerTrackManager::SetReferenceToSoundProducerTrackVector(std::vector <SoundProducerTrack*> *thisTrackVec)
{
	soundProducerTracks_vec = thisTrackVec;
}

void SoundProducerTrackManager::AddSourceOfLastTrackToSoundProducerTrackManager()
{
	if(soundProducerTracks_vec != nullptr)
	{
		ALuint* thisSource = soundProducerTracks_vec->at(soundProducerTracks_vec->size() - 1)->GetReferenceToTrackSource();
		soundproducertracks_sources_vector.push_back(thisSource);
	}
	
}
void SoundProducerTrackManager::RemoveSourceOfLastTrackFromSoundProducerTrackManager()
{
	soundproducertracks_sources_vector.pop_back();
}
	
//Functions inherited from Track



void SoundProducerTrackManager::SetReferenceToCurrentTimeVariable(double* thisTimeVariable){Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);}

double SoundProducerTrackManager::GetCurrentTime(){return Track::GetCurrentTime();}


//function to call in timer loop, variable to manipulate gets changed here
void SoundProducerTrackManager::FunctionToCallInPlayState()
{
	//buffer audio for all track sources
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInPlayState();
	}

	//play all sources in sync
	if(soundProducerTracks_vec->at(0)->GetReferenceToStereoAudioTrack()->GetAudioTrackState() == StereoAudioTrack::State::PLAYER_NULL)
	{
		audioPlayer->PlayMultipleSources(&soundproducertracks_sources_vector);
	}
	else if(soundProducerTracks_vec->at(0)->GetReferenceToStereoAudioTrack()->GetAudioTrackState() == StereoAudioTrack::State::PLAYER_PLAYING)
	{
		audioPlayer->PlayMultipleUpdatedPlayerBuffers(&soundproducertracks_sources_vector);
	}
}

void SoundProducerTrackManager::FunctionToCallInPauseState()
{
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInPauseState();
	}
}

void SoundProducerTrackManager::FunctionToCallInRewindState()
{
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInRewindState();
	}
}

void SoundProducerTrackManager::FunctionToCallInFastForwardState()
{
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInFastForwardState();
	}
}

void SoundProducerTrackManager::FunctionToCallInNullState()
{
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInNullState();
	}
}
