#include "XMLCreator.h"



XMLCreator::XMLCreator()
{
	
}

XMLCreator::~XMLCreator()
{
	
}

void XMLCreator::SaveDataToXMLFile(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr,
						   std::vector <SoundProducerTrack*> *ptrSPTracksVec,
						   std::vector <EchoZone> *echoZones,
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
    pugi::xml_node root = doc.append_child("BAEXMLRoot");
    
    
    XMLCreator::SaveDataXML_SPTracks(root,ptrSPTracksVec);
    
	XMLCreator::SaveDataXML_EffectZones(root,
								echoZones,
							    standardRevZones,
							    eaxRevZones);
	
	XMLCreator::SaveDataXML_SoundProducers(root,
											sound_producer_vector_ptr);				
	
	//write to file
	
	// Save XML tree to file.
    // Remark: second optional param is indent string to be used;
    // default indentation is tab character.
	bool saveSucceeded = doc.save_file(path.c_str(), PUGIXML_TEXT("  "));
	assert(saveSucceeded);
}

void XMLCreator::SaveDataXML_SPTracks(pugi::xml_node& root,
									  std::vector <SoundProducerTrack*> *ptrSPTracksVec)
{
	
	pugi::xml_node spTracksNode = root.append_child("SoundProducerTracks");
	
	for(size_t i = 0; i < ptrSPTracksVec->size(); i++)
	{

		pugi::xml_node nodeChild = spTracksNode.append_child("SoundProducerTrackChild");
		
		pugi::xml_node nameNodeChild = nodeChild.append_child("Name");
		nameNodeChild.append_attribute("name") = ptrSPTracksVec->at(i)->GetSoundProducerTrackSaveData().soundproducer_name.c_str();
		
		pugi::xml_node positionNodeChild = nodeChild.append_child("Filepath");
		positionNodeChild.append_attribute("filepath") = ptrSPTracksVec->at(i)->GetSoundProducerTrackSaveData().soundfilepath.c_str();
		
	}
	
	
}

void XMLCreator::SaveDataXML_EffectZones(pugi::xml_node& root,
								std::vector <EchoZone> *echoZones,
							    std::vector <ReverbZone> *standardRevZones,
							    std::vector <ReverbZone> *eaxRevZones)
{
	
	if(echoZones->size() > 0 || standardRevZones->size() > 0 || eaxRevZones->size() > 0)
	{
		//create effects zones nodes
		pugi::xml_node effectsNode = root.append_child("EffectZones");
		
		//get each effect zones save data and put it into xml file
		if(echoZones->size() > 0)
		{
			pugi::xml_node echoZonesNode = effectsNode.append_child("EchoZones");
			
			for(size_t i = 0; i < echoZones->size(); i++)
			{

				pugi::xml_node nodeChild = echoZonesNode.append_child("EchoZoneChild");
				
				pugi::xml_node nameNodeChild = nodeChild.append_child("Name");
				nameNodeChild.append_attribute("name") = echoZones->at(i).GetEchoZoneSaveData().name.c_str();
				
				pugi::xml_node positionNodeChild = nodeChild.append_child("Position");
				positionNodeChild.append_attribute("x") = echoZones->at(i).GetEchoZoneSaveData().x;
				positionNodeChild.append_attribute("y") = echoZones->at(i).GetEchoZoneSaveData().y;
				positionNodeChild.append_attribute("z") = echoZones->at(i).GetEchoZoneSaveData().z;
				
				pugi::xml_node sizeNodeChild = nodeChild.append_child("Size");
				sizeNodeChild.append_attribute("width") = echoZones->at(i).GetEchoZoneSaveData().width;
				
				pugi::xml_node propertyNodeChild = nodeChild.append_child("Property");
				propertyNodeChild.append_attribute("delay") = echoZones->at(i).GetEchoZoneSaveData().properties.flDelay;
				propertyNodeChild.append_attribute("lrdelay") = echoZones->at(i).GetEchoZoneSaveData().properties.flLRDelay;
				propertyNodeChild.append_attribute("damping") = echoZones->at(i).GetEchoZoneSaveData().properties.flDamping;
				propertyNodeChild.append_attribute("feedback") = echoZones->at(i).GetEchoZoneSaveData().properties.flFeedback;
				propertyNodeChild.append_attribute("spread") = echoZones->at(i).GetEchoZoneSaveData().properties.flSpread;
			}
		}
		
		if(standardRevZones->size() > 0)
		{
			pugi::xml_node standardRevZonesNode = effectsNode.append_child("StandardZones");
		
			for(size_t i = 0; i < standardRevZones->size(); i++)
			{
				pugi::xml_node nodeChild = standardRevZonesNode.append_child("StandardZoneChild");
				
				pugi::xml_node nameNodeChild = nodeChild.append_child("Name");
				nameNodeChild.append_attribute("name") = standardRevZones->at(i).GetStandardReverbZoneSaveData().name.c_str();
				
				pugi::xml_node positionNodeChild = nodeChild.append_child("Position");
				positionNodeChild.append_attribute("x") = standardRevZones->at(i).GetStandardReverbZoneSaveData().x;
				positionNodeChild.append_attribute("y") = standardRevZones->at(i).GetStandardReverbZoneSaveData().y;
				positionNodeChild.append_attribute("z") = standardRevZones->at(i).GetStandardReverbZoneSaveData().z;
				
				pugi::xml_node sizeNodeChild = nodeChild.append_child("Size");
				sizeNodeChild.append_attribute("width") = standardRevZones->at(i).GetStandardReverbZoneSaveData().width;
				
				pugi::xml_node propertyNodeChild = nodeChild.append_child("Property");
				propertyNodeChild.append_attribute("density") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flDensity;
				propertyNodeChild.append_attribute("diffusion") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flDiffusion;
				propertyNodeChild.append_attribute("gain") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flGain;
				propertyNodeChild.append_attribute("gainhf") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flGainHF;
				propertyNodeChild.append_attribute("decaytime") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flDecayTime;
				propertyNodeChild.append_attribute("decayhfratio") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flDecayHFRatio;
				propertyNodeChild.append_attribute("reflectgain") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flReflectionsGain;
				propertyNodeChild.append_attribute("reflectdelay") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flReflectionsDelay;
				propertyNodeChild.append_attribute("latereverbgain") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flLateReverbGain;
				propertyNodeChild.append_attribute("latereverbdelay") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flLateReverbDelay;
				propertyNodeChild.append_attribute("airgainhf") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flAirAbsorptionGainHF;
				propertyNodeChild.append_attribute("roomrolloff") = standardRevZones->at(i).GetStandardReverbZoneSaveData().properties.flRoomRolloffFactor;
			}
			
		}
		
		if(eaxRevZones->size() > 0)
		{
			pugi::xml_node eaxRevZonesNode = effectsNode.append_child("EAXZones");
		
			for(size_t i = 0; i < eaxRevZones->size(); i++)
			{
				pugi::xml_node nodeChild = eaxRevZonesNode.append_child("EAXZoneChild");
				
				pugi::xml_node nameNodeChild = nodeChild.append_child("Name");
				nameNodeChild.append_attribute("name") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().name.c_str();
				
				pugi::xml_node positionNodeChild = nodeChild.append_child("Position");
				positionNodeChild.append_attribute("x") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().x;
				positionNodeChild.append_attribute("y") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().y;
				positionNodeChild.append_attribute("z") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().z;
				
				pugi::xml_node sizeNodeChild = nodeChild.append_child("Size");
				sizeNodeChild.append_attribute("width") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().width;
				
				pugi::xml_node propertyNodeChild = nodeChild.append_child("Property");
				
				propertyNodeChild.append_attribute("density") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flDensity;
				propertyNodeChild.append_attribute("diffusion") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flDiffusion;
				propertyNodeChild.append_attribute("gain") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flGain;
				propertyNodeChild.append_attribute("gainhf") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flGainHF;
				propertyNodeChild.append_attribute("gainlf") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flGainLF;
				propertyNodeChild.append_attribute("decaytime") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flDecayTime;
				propertyNodeChild.append_attribute("decayhfratio") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flDecayHFRatio;
				propertyNodeChild.append_attribute("decaylfratio") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flDecayLFRatio;
				propertyNodeChild.append_attribute("reflectgain") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flReflectionsGain;
				propertyNodeChild.append_attribute("reflectdelay") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flReflectionsDelay;
				propertyNodeChild.append_attribute("latereverbgain") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flLateReverbGain;
				propertyNodeChild.append_attribute("latereverbdelay") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flLateReverbDelay;
				propertyNodeChild.append_attribute("echotime") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flEchoTime;
				propertyNodeChild.append_attribute("echodepth") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flEchoDepth;
				propertyNodeChild.append_attribute("modtime") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flModulationTime;
				propertyNodeChild.append_attribute("moddepth") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flModulationDepth;
				propertyNodeChild.append_attribute("hfref") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flHFReference;
				propertyNodeChild.append_attribute("lfref") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flLFReference;
				propertyNodeChild.append_attribute("airgainhf") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flAirAbsorptionGainHF;
				propertyNodeChild.append_attribute("roomrolloff") = eaxRevZones->at(i).GetEAXReverbZoneSaveData().properties.flRoomRolloffFactor;
			}
			
		}
	}
	
}

void XMLCreator::SaveDataXML_SoundProducers(pugi::xml_node& root,
									std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ptr)
{
	
	if( sound_producer_vector_ptr->size() > 0)
	{
		//create sound producers node
		pugi::xml_node spNode = root.append_child("SoundProducers");

		for(size_t i = 0; i < sound_producer_vector_ptr->size(); i++)
		{
			pugi::xml_node spNodeChild = spNode.append_child("SoundProducerChild");
			
			pugi::xml_node nameNodeChild = spNodeChild.append_child("Name");
			nameNodeChild.append_attribute("name") = sound_producer_vector_ptr->at(i)->GetSoundProducerSaveData().name.c_str();
			
			pugi::xml_node positionNodeChild = spNodeChild.append_child("Position");
			positionNodeChild.append_attribute("x") = sound_producer_vector_ptr->at(i)->GetSoundProducerSaveData().x;
			positionNodeChild.append_attribute("y") = sound_producer_vector_ptr->at(i)->GetSoundProducerSaveData().y;
			positionNodeChild.append_attribute("z") = sound_producer_vector_ptr->at(i)->GetSoundProducerSaveData().z;
			
			//filename
			
			
		}
				
	}
	
	
}
