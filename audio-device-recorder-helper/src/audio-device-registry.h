#ifndef AUDIO_DEVICE_REGISTRY_H
#define AUDIO_DEVICE_REGISTRY_H

#include <wx/arrstr.h>
#include <unordered_map>

#include <wx/combobox.h>
#include <vector>

class AudioDeviceRegistry
{
public:
	AudioDeviceRegistry();
	~AudioDeviceRegistry();
	
	//function to update list of audio devices
	void UpdateListOfAudioDevices();
	
	//function to add reference to combo box to combo_box_ptr_vec
	void AddReferenceToComboBox(wxComboBox* thisComboBox);
	
	//function to remove a combo box pointer from container, use when soundproducer track is removed
	void RemoveLastComboBoxReference();
	
	//function to update all comboboxes list, used for updating list after removal of a selection that is being edited by a soundproducer track
	void UpdateAllComboBoxesList();
	
	//function to return list of audio devices
	wxArrayString& GetListofAudioDevices();
	
private:
	
	//list of names for combo box
	wxArrayString audio_devices_list_wxstring;
	
	//vector of pointers to combo boxes
	//used to update all combo boxes
	std::vector <wxComboBox*> combo_box_ptr_vec;
};

#endif
