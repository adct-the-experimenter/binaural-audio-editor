#ifndef XML_READER_H
#define XML_READER_H

#include "pugixml.hpp"
#include <iostream>

#include <string>
#include <vector>

#include "listener-track.h"
#include "soundproducer.h"
#include "soundproducer-track.h"
#include "echo-zone.h"
#include "reverb-zone.h"



//class used to save data in binaural audio editor into a xml file

class XMLReader
{
public:
	
	XMLReader();
	~XMLReader();
	
	void LoadDataFromXMLFile(std::vector <SoundProducerSaveData> *sound_producer_save_data,
							   std::vector <SoundProducerTrackSaveData> *ptrSPTracksVec,
							   std::vector <EchoZoneSaveData> *echoZonesSaveData,
							   std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData,
							   std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData,
							   ListenerTrackSaveData& listener_track_ptr,
							   ListenerSaveData& listener_data,
							   std::string path);
	
private:
	
	void LoadData_SoundProducers(pugi::xml_node& root, std::vector <SoundProducerSaveData> *sound_producer_save_data);
	void LoadData_SoundProducerTracks(pugi::xml_node& root,
									std::vector <SoundProducerTrackSaveData> *ptrSPTracksVec);
	void LoadData_EchoZones(pugi::xml_node& root,std::vector <EchoZoneSaveData> *echoZonesSaveData);
	void LoadData_StandardRevZones(pugi::xml_node& root,std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData);
	void LoadData_EAXRevZones(pugi::xml_node& root,std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData);
	
	void LoadData_ListenerTrack(pugi::xml_node& root, ListenerTrackSaveData& listener_track_data);
	
	void LoadData_Listener(pugi::xml_node& root, ListenerSaveData& listener_data);
};

#endif
