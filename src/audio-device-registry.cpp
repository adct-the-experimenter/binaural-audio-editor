#include "audio-device-registry.h"

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft

#include <iostream>

AudioDeviceRegistry::AudioDeviceRegistry()
{
	
}

AudioDeviceRegistry::~AudioDeviceRegistry()
{
	
}

void AudioDeviceRegistry::UpdateListOfAudioDevices()
{
	
	const ALchar *pDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
	
	if (pDeviceList)
	{
		//printf("Available Capture Devices are:-\n\n");

		while (*pDeviceList)
		{
		   //printf("%s\n", pDeviceList);
		   
		   wxString thisString( pDeviceList );

		   audio_devices_list_wxstring.Add(thisString);
		   
		   pDeviceList += strlen(pDeviceList) + 1;
		   
		}
		
		
		/*
		 * Check contents of array
		std::cout << "devices in string:\n";
		
		for(int i = 0; i < audio_devices_list_wxstring.size(); i++)
		{
			std::cout << audio_devices_list_wxstring[i] << "i:" << i << std::endl;
		}
		*/
	} 
	
}

void AudioDeviceRegistry::AddReferenceToComboBox(wxComboBox* thisComboBox)
{
	combo_box_ptr_vec.push_back(thisComboBox);
}
	
void AudioDeviceRegistry::RemoveLastComboBoxReference()
{
	combo_box_ptr_vec.pop_back();
}
	
void AudioDeviceRegistry::UpdateAllComboBoxesList()
{
	for(size_t i=0; i < combo_box_ptr_vec.size(); i++)
	{
		wxString currentNameSelected = combo_box_ptr_vec[i]->GetStringSelection();

		combo_box_ptr_vec[i]->Clear();
		combo_box_ptr_vec[i]->Append(audio_devices_list_wxstring);

		int select_index = combo_box_ptr_vec[i]->FindString(currentNameSelected,true); //case sensitive = true
		combo_box_ptr_vec[i]->SetSelection(select_index);
	}
}
