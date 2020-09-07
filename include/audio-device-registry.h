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
	
private:
	
	//list of names for combo box
	wxArrayString audio_devices_list_wxstring;
	
	//vector of pointers to combo boxes
	//used to update all combo boxes
	std::vector <wxComboBox*> combo_box_ptr_vec;
};

#endif
