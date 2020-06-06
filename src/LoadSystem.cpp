#include "LoadSystem.h"

LoadSystem::LoadSystem()
{
	
}

LoadSystem::~LoadSystem()
{
	
}
	
void LoadSystem::LoadProject(std::vector <SoundProducerSaveData> *sound_producer_save_data,
							   std::vector <SoundProducerTrackSaveData> *ptrSPTracksSaveDataVec,
							   std::vector <EchoZoneSaveData> *echoZonesSaveData,
							   std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData,
							   std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData,
							   std::string path
							 )
{
	//read data from xml file and push into the sound producers, sound producer tracks, 
	//and effect zones
								
	xml_reader.LoadDataFromXMLFile(sound_producer_save_data,
							   ptrSPTracksSaveDataVec,
							   echoZonesSaveData,
							   standardRevZonesSaveData,
							   eaxRevZonesSaveData,
							   path);
	
	
}
