#include "audio-device-registry.h"

#include "RtAudio.h"

#include <iostream>

AudioDeviceRegistry::AudioDeviceRegistry()
{
	
}

AudioDeviceRegistry::~AudioDeviceRegistry()
{
	
}

void AudioDeviceRegistry::UpdateListOfAudioDevices()
{
	
	RtAudio audio;
	RtAudio::DeviceInfo info;
	
	if ( audio.getDeviceCount() < 1 ) 
	{
		std::cout << "\nNo audio devices.\n";
	}
	else
	{
		for (unsigned int i=0;i < audio.getDeviceCount();i++)
		{
			info = audio.getDeviceInfo( i );
			
			wxString thisString( info.name );

			audio_devices_list_wxstring.Add(thisString);	
		}
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
