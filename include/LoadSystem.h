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
	void LoadProject(std::string path);
	
private:
	
	//function to create effect zones based on XML file content
	void LoadEffectsZones(EffectsManager* effectsManagerPtr, std::string path);
	
};

#endif
