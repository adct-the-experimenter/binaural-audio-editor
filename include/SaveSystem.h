#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <memory>

#include "XMLCreator.h"
#include "effects-manager.h"


class SaveSystem
{
public:
	SaveSystem();
	~SaveSystem();
	
	//function used to save project data to already established save file
	void SaveProjectToSetFile(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr,
							  EffectsManager* effectsManagerPtr,
							  ListenerTrack* listener_track_ptr,
							  Listener* listener_ptr);
	
	//function to save project data to new file
	//overwrites member save file path
	void SaveAsNewProject(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr,
						  EffectsManager* effectsManagerPtr,
						  ListenerTrack* listener_track_ptr,
						  Listener* listener_ptr, 
						  std::string path);
	
	//function to set svae file path
	void SetSaveFilePath(std::string path);
	
private:
	
	//xml file writer
	XMLCreator xml_creator;
	
	std::string m_saveFilePath;
	
	void SaveProject(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr,
					 EffectsManager* effectsManagerPtr,
					 ListenerTrack* listener_track_ptr,
					 Listener* listener_ptr,  
					 std::string path);
};

#endif
