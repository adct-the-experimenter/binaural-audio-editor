#include "soundproducer-track-manager.h"

SoundProducerTrackManager::SoundProducerTrackManager(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext) : Track(title)
{
	//initialize audio player
	audioPlayer = new OpenALSoftPlayer();
	audioPlayer->SetReferenceToAudioContext(thisAudioContext);
	audioPlayer->SetReferenceToAudioDevice(thisAudioDevice);
	
	soundProducerTracks_vec = nullptr;
	
	SoundProducerTrackManager::SetSoundPlayingBool(false);
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
	SoundProducerTrackManager::SetSoundPlayingBool(true);
	
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
	SoundProducerTrackManager::SetSoundPlayingBool(false);
	
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInPauseState();
	}
}

void SoundProducerTrackManager::FunctionToCallInRewindState()
{
	SoundProducerTrackManager::SetSoundPlayingBool(false);
	
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInRewindState();
	}
}

void SoundProducerTrackManager::FunctionToCallInFastForwardState()
{
	SoundProducerTrackManager::SetSoundPlayingBool(false);
	
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInFastForwardState();
	}
}

void SoundProducerTrackManager::FunctionToCallInNullState()
{
	SoundProducerTrackManager::SetSoundPlayingBool(false);
	
	for(size_t i=0; i < soundProducerTracks_vec->size(); i++)
	{
		soundProducerTracks_vec->at(i)->FunctionToCallInNullState();
	}
}

void SoundProducerTrackManager::SetSoundPlayingBool(bool status){soundPlaying = status;}

bool SoundProducerTrackManager::IsSoundBeingPlayed(){return soundPlaying;}

void SoundProducerTrackManager::PlayThisTrackFromSoundProducerTrackVector(int& index)
{
	//buffer audio for track source
	//soundProducerTracks_vec->at(index)->FunctionToCallInPlayState();
	
	double current_time = 0;
	
	soundProducerTracks_vec->at(index)->BufferAndPlayAudio(current_time);
	
}

void SoundProducerTrackManager::PauseThisTrackFromSoundProducerTrackVector(int& index)
{
	audioPlayer->PauseSource(soundproducertracks_sources_vector[index]);
}

void SoundProducerTrackManager::StopThisTrackFromSoundProducerTrackVector(int& index)
{
	soundProducerTracks_vec->at(index)->StopAudio();
}

void SoundProducerTrackManager::BrowseAudioForThisSoundProducer(SoundProducer* lastProducer)
{
	//get sound producer name of last sound producer created
	soundProducerTracks_vec->back()->SelectSoundProducerByName(lastProducer->GetNameString());
	
	soundProducerTracks_vec->back()->GetReferenceToStereoAudioTrack()->BrowseForInputAudioFile();
}
