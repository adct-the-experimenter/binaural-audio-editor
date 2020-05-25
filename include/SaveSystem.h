#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "effects-manager.h"
#include "XMLCreator.h"

class SaveSystem
{
public:
	SaveSystem();
	~SaveSystem();
	
	//function used to save project data to already established save file
	void SaveProjectToSetFile(EffectsManager* effectsManagerPtr);
	
	//function to save project data to new file
	//overwrites member save file path
	void SaveAsNewProject(EffectsManager* effectsManagerPtr,std::string path);
	
	//function to set svae file path
	void SetSaveFilePath(std::string path);
	
private:
	
	//function to save effect zone data to xml file
	void SaveEffectsZones(EffectsManager* effectsManagerPtr, std::string path);
	
	XMLCreator xml_creator;
	
	std::string m_saveFilePath;
	
	void SaveProject(EffectsManager* effectsManagerPtr, std::string path);
};

#endif
