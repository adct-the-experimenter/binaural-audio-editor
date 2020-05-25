#ifndef XML_READER_H
#define XML_READER_H

#include <string>

//class used to save data in binaural audio editor into a xml file

class XMLReader
{
public:
	
	XMLReader();
	~XMLReader();
	
	void LoadDataFromXMLFile(std::string path);
	
private:
	
};

#endif
