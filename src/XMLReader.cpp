#include "XMLReader.h"

#include <cstdlib>

XMLReader::XMLReader()
{
	
}

XMLReader::~XMLReader()
{
	
}
	
void XMLReader::LoadDataFromXMLFile(std::vector <SoundProducerSaveData> *sound_producer_save_data,
							   std::vector <SoundProducerTrackSaveData> *ptrSPTracksSaveDataVec,
							   std::vector <EchoZoneSaveData> *echoZonesSaveData,
							   std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData,
							   std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData,
							   std::string path)
{
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(path.c_str(),
												pugi::parse_default);
    if (!result)
    {
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return;
    }
    
    pugi::xml_node root = doc.child("BAEXMLRoot");
    
    XMLReader::LoadData_SoundProducers(root, sound_producer_save_data);
	XMLReader::LoadData_SoundProducerTracks(root,ptrSPTracksSaveDataVec);
	XMLReader::LoadData_EchoZones(root,echoZonesSaveData);
	XMLReader::LoadData_StandardRevZones(root,standardRevZonesSaveData);
	XMLReader::LoadData_EAXRevZones(root,eaxRevZonesSaveData);
	
	
}

void XMLReader::LoadData_SoundProducers(pugi::xml_node& root, std::vector <SoundProducerSaveData> *sound_producer_save_data)
{
	pugi::xml_node spRoot = root.child("SoundProducers");
	
	
	//go through each sound producer node
	for (pugi::xml_node sp_node = spRoot.first_child(); sp_node; sp_node = sp_node.next_sibling() )
	{	
		//DungeonTile::TileType type = DungeonTile::TileType::RED;
		SoundProducerSaveData data;
		
		std::string valString; 
		
		valString = sp_node.child("Name").attribute("name").value();
		data.name = valString;
		
		valString = sp_node.child("Position").attribute("x").value();
		data.x = atof(valString.c_str());
		
		valString = sp_node.child("Position").attribute("y").value();
		data.y = atof(valString.c_str());
		
		valString = sp_node.child("Position").attribute("z").value();
		data.z = atof(valString.c_str());
		
		sound_producer_save_data->push_back(data);		
	}
		
	
}

void XMLReader::LoadData_SoundProducerTracks(pugi::xml_node& root,std::vector <SoundProducerTrackSaveData> *ptrSPTracksSaveDataVec)
{
	
}

void XMLReader::LoadData_EchoZones(pugi::xml_node& root,std::vector <EchoZoneSaveData> *echoZonesSaveData)
{
	
}

void XMLReader::LoadData_StandardRevZones(pugi::xml_node& root,std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData)
{
	
}

void XMLReader::LoadData_EAXRevZones(pugi::xml_node& root,std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData)
{
	
}
