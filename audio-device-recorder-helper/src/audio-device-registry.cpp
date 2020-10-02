#include "audio-device-registry.h"

#include <iostream>

#include "portaudio.h"

AudioDeviceRegistry::AudioDeviceRegistry()
{
	m_num_devices = 0;
}

AudioDeviceRegistry::~AudioDeviceRegistry()
{
	
}

void AudioDeviceRegistry::UpdateListOfAudioDevices()
{
	
    const   PaDeviceInfo *deviceInfo;
    PaStreamParameters inputParameters, outputParameters;
    PaError err;
   
       
    err = Pa_Initialize();
    if( err != paNoError )
    {
		printf( "ERROR: Pa_Initialize returned 0x%x\n", err );
    }
       
    printf( "PortAudio version: 0x%08X\n", Pa_GetVersion());
    printf( "Version text: '%s'\n", Pa_GetVersionInfo()->versionText );
   
    int numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
        err = numDevices;
    }
    else
    {
		m_num_devices = numDevices;
		
		for(size_t i = 0; i < m_num_devices; i++)
		{
			deviceInfo = Pa_GetDeviceInfo( i );
			
			wxString thisString( deviceInfo->name );

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

wxArrayString& AudioDeviceRegistry::GetListofAudioDevices(){return audio_devices_list_wxstring;}
