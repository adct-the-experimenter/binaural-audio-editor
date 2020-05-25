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
    pugi::xml_node tilesNode = root.append_child("EffectZones");
	
	//get each effect zones save data and put it into xml file
	
	//write to file
	
	// Save XML tree to file.
    // Remark: second optional param is indent string to be used;
    // default indentation is tab character.
	bool saveSucceeded = doc.save_file(path.c_str(), PUGIXML_TEXT("  "));
	assert(saveSucceeded);
}
