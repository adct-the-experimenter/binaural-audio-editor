#ifndef LOAD_SYSTEM_H
#define LOAD_SYSTEM_H

#include <string>

#include "XMLReader.h"
#include "effects-manager.h"


class LoadSystem
{
public:
	LoadSystem();
	~LoadSystem();
	
	//function used to load project
	void LoadProject(std::vector <SoundProducerSaveData> *sound_producer_save_data,
							   std::vector <SoundProducerTrackSaveData> *ptrSPTracksSaveDataVec,
							   std::vector <EchoZoneSaveData> *echoZonesSaveData,
							   std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData,
							   std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData,
							   ListenerTrackSaveData& listener_track_data,
							   ListenerSaveData& listener_save_data,
							   std::string path);
	
private:
	
	//xml file reader
	XMLReader xml_reader;
	
	//for sound producer tracks 
	//std::vector <DDMap*> tmp_DDXMaps;
	//std::vector <DDMap*> tmp_DDYMaps;
	//std::vector <DDMap*> tmp_DDZMaps;
	
	
};

#endif
