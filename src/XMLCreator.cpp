#include "XMLCreator.h"

#include "pugixml.hpp"
#include <iostream>

XMLCreator::XMLCreator()
{
	
}

XMLCreator::~XMLCreator()
{
	
}

void XMLCreator::SaveDataToXMLFile(std::vector <EchoZone> *echoZones,
						   std::vector <ReverbZone> *standardRevZones,
						   std::vector <ReverbZone> *eaxRevZones,
						   std::string path)
{
	
	// Generate new XML document within memory
    pugi::xml_document doc;
    
    // Generate XML declaration
    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version")    = "1.0";
    declarationNode.append_attribute("encoding")   = "ISO-8859-1";
    declarationNode.append_attribute("standalone") = "yes";
    
    // A valid XML doc must contain a single root node of any name
    auto root = doc.append_child("BAEXMLRoot");
    
    //create effects zones nodes
    pugi::xml_node effectsNode = root.append_child("EffectZones");
	
	
	
	//get each effect zones save data and put it into xml file
	
	if(echoZones->size() > 0)
	{
		pugi::xml_node echoZonesNode = effectsNode.append_child("EchoZones");
		
		for(size_t i = 0; i < echoZones->size(); i++)
		{

			pugi::xml_node nodeChild = echoZonesNode.append_child("EchoZoneChild");
			
			pugi::xml_node nameNodeChild = nodeChild.append_child("Name");
			nameNodeChild.append_attribute("name") = echoZones->at(i).GetEchoZoneSaveData().name.c_str();
			
			pugi::xml_node positionNodeChild = nodeChild.append_child("Position");
			positionNodeChild.append_attribute("x") = echoZones->at(i).GetEchoZoneSaveData().x;
			positionNodeChild.append_attribute("y") = echoZones->at(i).GetEchoZoneSaveData().y;
			positionNodeChild.append_attribute("z") = echoZones->at(i).GetEchoZoneSaveData().z;
			
			pugi::xml_node sizeNodeChild = nodeChild.append_child("Size");
			sizeNodeChild.append_attribute("width") = echoZones->at(i).GetEchoZoneSaveData().width;
			
			pugi::xml_node propertyNodeChild = nodeChild.append_child("Property");
			
			
		}
	}
	
	if(standardRevZones->size() > 0)
	{
		pugi::xml_node standardRevZonesNode = effectsNode.append_child("StandardZones");
	
		for(size_t i = 0; i < standardRevZones->size(); i++)
		{
			pugi::xml_node nodeChild = standardRevZonesNode.append_child("StandardZoneChild");
			
			pugi::xml_node nameNodeChild = nodeChild.append_child("Name");
			nameNodeChild.append_attribute("name") = standardRevZones->at(i).GetStandardReverbZoneSaveData().name.c_str();
			
			pugi::xml_node positionNodeChild = nodeChild.append_child("Position");
			positionNodeChild.append_attribute("x") = standardRevZones->at(i).GetStandardReverbZoneSaveData().x;
			positionNodeChild.append_attribute("y") = standardRevZones->at(i).GetStandardReverbZoneSaveData().y;
			positionNodeChild.append_attribute("z") = standardRevZones->at(i).GetStandardReverbZoneSaveData().z;
			
			pugi::xml_node sizeNodeChild = nodeChild.append_child("Size");
			sizeNodeChild.append_attribute("width") = standardRevZones->at(i).GetStandardReverbZoneSaveData().width;
			
			pugi::xml_node propertyNodeChild = nodeChild.append_child("Property");
		}
		
	}
	
	if(eaxRevZones->size() > 0)
	{
		pugi::xml_node eaxRevZonesNode = effectsNode.append_child("EAXZones");
	
		for(size_t i = 0; i < eaxRevZones->size(); i++)
		{
			pugi::xml_node nodeChild = eaxRevZonesNode.append_child("EAXZoneChild");
			
			pugi::xml_node nameNodeChild = nodeChild.append_child("Name");
			nameNodeChild.append_attribute("name") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().name.c_str();
			
			pugi::xml_node positionNodeChild = nodeChild.append_child("Position");
			positionNodeChild.append_attribute("x") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().x;
			positionNodeChild.append_attribute("y") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().y;
			positionNodeChild.append_attribute("z") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().z;
			
			pugi::xml_node sizeNodeChild = nodeChild.append_child("Size");
			sizeNodeChild.append_attribute("width") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().width;
			
			pugi::xml_node propertyNodeChild = nodeChild.append_child("Property");
		}
		
	}
	
	//write to file
	
	// Save XML tree to file.
    // Remark: second optional param is indent string to be used;
    // default indentation is tab character.
	bool saveSucceeded = doc.save_file(path.c_str(), PUGIXML_TEXT("  "));
	assert(saveSucceeded);
}
