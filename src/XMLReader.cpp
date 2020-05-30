#include "XMLReader.h"

XMLReader::XMLReader()
{
	
}

XMLReader::~XMLReader()
{
	
}
	
void XMLReader::LoadDataFromXMLFile(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr,
								   std::vector <SoundProducerTrack*> *ptrSPTracksVec,
								   std::vector <EchoZone> *echoZones,
								   std::vector <ReverbZone> *standardRevZones,
								   std::vector <ReverbZone> *eaxRevZones,
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
    
    XMLReader::LoadData_SoundProducers(root, sound_producer_vector_ptr);
	XMLReader::LoadData_SoundProducerTracks(root,ptrSPTracksVec);
	XMLReader::LoadData_EchoZones(root,echoZones);
	XMLReader::LoadData_StandardRevZones(root,standardRevZones);
	XMLReader::LoadData_EAXRevZones(root,eaxRevZones);
	
    //pugi::xml_node tileRoot = doc.child("BAEXMLRoot").child("Tiles");
	
	/*
	//go through each tile type in tiles node
	for (pugi::xml_node tileType = root.first_child(); tileType; tileType = tileType.next_sibling())
	{	
		DungeonTile::TileType type = DungeonTile::TileType::RED;
		
		std::string valString = tileType.first_child().value();
		//std::cout << valString << std::endl;
		
	}
	*/
	
}

void XMLReader::LoadData_SoundProducers(pugi::xml_node& root, std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr)
{
	
}

void XMLReader::LoadData_SoundProducerTracks(pugi::xml_node& root,std::vector <SoundProducerTrack*> *ptrSPTracksVec)
{
	
}

void XMLReader::LoadData_EchoZones(pugi::xml_node& root,std::vector <EchoZone> *echoZones)
{
	
}

void XMLReader::LoadData_StandardRevZones(pugi::xml_node& root,std::vector <ReverbZone> *standardRevZones)
{
	
}

void XMLReader::LoadData_EAXRevZones(pugi::xml_node& root,std::vector <ReverbZone> *eaxRevZones)
{
	
}
