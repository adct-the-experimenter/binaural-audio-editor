#ifndef XMLCREATOR_H
#define XMLCREATOR_H

#include <string>

//class used to save data in binaural audio editor into a xml file

class XMLCreator
{
public:
	
	XMLCreator();
	~XMLCreator();
	
	void SaveDataToXMLFile(std::string path);
	
private:
	
};

#endif
