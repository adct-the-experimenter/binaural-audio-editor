#ifndef XMLCREATOR_H
#define XMLCREATOR_H

#include <string>

#include "echo-zone.h"
#include "reverb-zone.h"

//class used to save data in binaural audio editor into a xml file

class XMLCreator
{
public:
	
	XMLCreator();
	~XMLCreator();
	
	void SaveDataToXMLFile(std::vector <EchoZone> *echoZones,
						   std::vector <ReverbZone> *standardRevZones,
						   std::vector <ReverbZone> *eaxRevZones,
						   std::string path);
	
private:
	
};

#endif
