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
