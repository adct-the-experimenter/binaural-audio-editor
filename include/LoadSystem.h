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
	void LoadProject(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr,
							 EffectsManager* effectsManagerPtr, 
							 std::string path);
	
private:
	
	//xml file reader
	XMLReader xml_reader;
	
};

#endif
