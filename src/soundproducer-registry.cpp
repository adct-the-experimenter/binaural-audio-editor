#include "soundproducer-registry.h"


void SoundProducerRegistry::SetReferenceToSoundProducerVector(std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector)
{
	sound_producer_vector_ref = sound_producer_vector;
}

SoundProducer* SoundProducerRegistry::GetPointerToSoundProducerWithThisName(std::string thisName)
{
	//get iterator to sound producers vector from soundproducers map
	std::unordered_map<std::string,std::vector <std::unique_ptr <SoundProducer> >::iterator>::const_iterator got = map_soundproducer.find (thisName);
	
	std::vector <std::unique_ptr <SoundProducer> >::iterator it = got->second;
	
	return it->get();
}

wxArrayString& SoundProducerRegistry::GetSoundProducersToEditList(){return soundproducers_to_edit_wxstring;}

void SoundProducerRegistry::AddRecentSoundProducerMadeToRegistry()
{
	if(sound_producer_vector_ref != nullptr)
	{
		//std::cout << "Add recent sound producer made to track called! \n";
		
		SoundProducer* thisSoundProducer = nullptr;
		std::vector <std::unique_ptr <SoundProducer> >::iterator it;
		
		if(sound_producer_vector_ref->size() == 1)
		{
			thisSoundProducer = sound_producer_vector_ref->at(0).get();
			it = sound_producer_vector_ref->begin();
		}
		else if(sound_producer_vector_ref->size() > 1)
		{
			thisSoundProducer = sound_producer_vector_ref->at(sound_producer_vector_ref->size() - 1).get();
			it = sound_producer_vector_ref->end();
		}
		
		if(thisSoundProducer != nullptr)
		{
			wxString thisString(thisSoundProducer->GetNameString());
			
			soundproducers_to_edit_wxstring.Add(thisString);
			
			map_soundproducer.emplace(thisSoundProducer->GetNameString(),it);
		}
		
	}
	
}

void SoundProducerRegistry::RemoveSoundProducerFromRegistry(SoundProducer* thisSoundProducer)
{
	wxString thisString(thisSoundProducer->GetNameString());
	soundproducers_to_edit_wxstring.Remove(thisString);
	
	map_soundproducer.erase(thisSoundProducer->GetNameString());
}
