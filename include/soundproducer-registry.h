#ifndef SOUND_PRODUCER_REGISTRY
#define SOUND_PRODUCER_REGISTRY

#include "soundproducer.h"
#include <memory>
#include <wx/arrstr.h>
#include <unordered_map>

#include <wx/combobox.h>
#include <list>

//class to contain map of sound producer pointers and their keys

class SoundProducerRegistry
{
public:

	void SetReferenceToSoundProducerVector(std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector);
	
	SoundProducer* GetPointerToSoundProducerWithThisName(std::string thisName);
	
	//Add or remove sound producer from registry
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
	
	//function to add all names of soundproducers in map to editor list
	//use with RemoveThisNameFromEditingList to get back a soundproducer to editing list after it is not edited in soundproducer track
	void AddAllSoundProducersToEditingList(); 
	
	//function to add reference to combo box to combo_box_ptr_vec
	void AddReferenceToComboBox(wxComboBox* thisComboBox);
	
	//function to remove a combo box pointer from container, use when soundproducer track is removed
	void RemoveLastComboBoxReference();
	
	//function to update all comboboxes list, used for updating list after removal of a selection that is being edited by a soundproducer track
	void UpdateAllComboBoxesList();
	
	//function to remove a string name from all combo boxes except for one
	void RemoveThisNameFromAllComboBoxesExceptThisOne(std::string thisName, wxComboBox* thisCombobox);
	
	//function to clear all names from registry
	void ClearAllSoundproducerNames();
	
	//function to clear all
	void ClearAll();
	
private:

	//list of names for combo box
	wxArrayString soundproducers_to_edit_wxstring;
	
	//pointer to vector of sound producers to edit
	std::vector <std::unique_ptr <SoundProducer> > *sound_producer_vector_ref; 
	
	//vector of pointers to combo boxes
	//used to update all combo boxes
	std::vector <wxComboBox*> combo_box_ptr_vec;
	
	//dictionary to keep track of which wxstring associated with index
	//std::unordered_map <std::string, std::vector <std::unique_ptr <SoundProducer> >::iterator> map_soundproducer;
	std::unordered_map <std::string, size_t> map_soundproducer; 
};

#endif
