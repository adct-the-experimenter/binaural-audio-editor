#ifndef SOUND_PRODUCER_REGISTRY
#define SOUND_PRODUCER_REGISTRY

#include "soundproducer.h"
#include <memory>
#include <wx/arrstr.h>
#include <unordered_map>

//class to contain map of sound producer pointers and their keys

class SoundProducerRegistry
{
public:

	void SetReferenceToSoundProducerVector(std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector);
	
	SoundProducer* GetPointerToSoundProducerWithThisName(std::string thisName);
	
	void AddRecentSoundProducerMadeToRegistry();
	void RemoveSoundProducerFromRegistry(SoundProducer* thisSoundProducer);
	
	//function to return list of sound producers to edit
	wxArrayString& GetSoundProducersToEditList();
	
	//function to remove a name from the list of sound producers to edit
	//use if a name is already being edited by a track
	void RemoveThisNameFromEditingList(std::string thisName);
	
	//function to add a name from the list of sound producers to edit
	//use if a name was removed for editing because it was being edited and then it stopped being edited
	void AddThisNameToEditingList(std::string thisName);
	
private:

	//list of names for combo box
	wxArrayString soundproducers_to_edit_wxstring;
	
	//pointer to vector of sound producers to edit
	std::vector <std::unique_ptr <SoundProducer> > *sound_producer_vector_ref; 
	
	//dictionary to keep track of which wxstring associated with index
	std::unordered_map <std::string, std::vector <std::unique_ptr <SoundProducer> >::iterator> map_soundproducer; 
};

#endif
