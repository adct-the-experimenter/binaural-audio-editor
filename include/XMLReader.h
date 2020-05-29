#ifndef XML_READER_H
#define XML_READER_H

#include "pugixml.hpp"
#include <iostream>

#include <string>
#include <vector>

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
	
	void LoadDataFromXMLFile(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr,
							   std::vector <SoundProducerTrack*> *ptrSPTracksVec,
							   std::vector <EchoZone> *echoZones,
							   std::vector <ReverbZone> *standardRevZones,
							   std::vector <ReverbZone> *eaxRevZones,
							   std::string path);
	
private:
	
	void LoadData_SoundProducers();
	
};

#endif
