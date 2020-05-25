#include "SaveSystem.h"

SaveSystem::SaveSystem()
{
	
}

SaveSystem::~SaveSystem()
{
	
}

void SaveSystem::SaveProjectToSetFile(EffectsManager* effectsManagerPtr)
{
	SaveSystem::SaveProject(effectsManagerPtr,m_saveFilePath);
}
	
void SaveSystem::SaveAsNewProject(EffectsManager* effectsManagerPtr,std::string path)
{
	m_saveFilePath = path;
	SaveSystem::SaveProject(effectsManagerPtr,m_saveFilePath);
}
	
void SaveSystem::SaveProject(EffectsManager* effectsManagerPtr, std::string path)
{
	std::cout << "Save project called!\n";
	
	SaveSystem::SaveEffectsZones(effectsManagerPtr,path);
}

void SaveSystem::SaveEffectsZones(EffectsManager* effectsManagerPtr, std::string path)
{
	//for each specific effect zone type
	//xml_creator
	
}

void SaveSystem::SetSaveFilePath(std::string path){m_saveFilePath = path;}
