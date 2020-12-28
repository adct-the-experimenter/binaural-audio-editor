#include "XMLReader.h"

#include <cstdlib>

XMLReader::XMLReader()
{
	
}

XMLReader::~XMLReader()
{
	
}
	
void XMLReader::LoadDataFromXMLFile(std::vector <SoundProducerSaveData> *sound_producer_save_data,
							   std::vector <SoundProducerTrackSaveData> *ptrSPTracksSaveDataVec,
							   std::vector <EchoZoneSaveData> *echoZonesSaveData,
							   std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData,
							   std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData,
							   ListenerTrackSaveData& listener_track_data,
							   ListenerSaveData& listener_data,
							   std::string path)
{
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(path.c_str(),
												pugi::parse_default);
    if (!result)
    {
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return;
    }
    
    pugi::xml_node root = doc.child("BAEXMLRoot");
    
    XMLReader::LoadData_SoundProducers(root, sound_producer_save_data);
	XMLReader::LoadData_SoundProducerTracks(root,ptrSPTracksSaveDataVec);
	XMLReader::LoadData_EchoZones(root,echoZonesSaveData);
	XMLReader::LoadData_StandardRevZones(root,standardRevZonesSaveData);
	XMLReader::LoadData_EAXRevZones(root,eaxRevZonesSaveData);
	
	XMLReader::LoadData_ListenerTrack(root,listener_track_data);
	XMLReader::LoadData_Listener(root,listener_data);
}

void XMLReader::LoadData_SoundProducers(pugi::xml_node& root, std::vector <SoundProducerSaveData> *sound_producer_save_data)
{
	pugi::xml_node spRoot = root.child("SoundProducers");
	
	
	//go through each sound producer node
	for (pugi::xml_node sp_node = spRoot.first_child(); sp_node; sp_node = sp_node.next_sibling() )
	{	
		//DungeonTile::TileType type = DungeonTile::TileType::RED;
		SoundProducerSaveData data;
		
		std::string valString; 
		
		valString = sp_node.child("Name").attribute("name").value();
		data.name = valString;
		
		valString = sp_node.child("Position").attribute("x").value();
		data.x = atof(valString.c_str());
		
		valString = sp_node.child("Position").attribute("y").value();
		data.y = atof(valString.c_str());
		
		valString = sp_node.child("Position").attribute("z").value();
		data.z = atof(valString.c_str());
		
		sound_producer_save_data->push_back(data);		
	}
		
	
}

void XMLReader::LoadData_SoundProducerTracks(pugi::xml_node& root,
											 std::vector <SoundProducerTrackSaveData> *ptrSPTracksSaveDataVec)
{
	pugi::xml_node spTrackRoot = root.child("SoundProducerTracks");
	
	for(pugi::xml_node track_node = spTrackRoot.first_child(); track_node; track_node = track_node.next_sibling() )
	{
		
		SoundProducerTrackSaveData data;
		std::string valString;
		
		valString = track_node.child("Name").attribute("name").value();
		data.soundproducer_name = valString;
		
		valString = track_node.child("Filepath").attribute("filepath").value();
		data.soundfilepath = valString;

		pugi::xml_node positionXTimeNodeChild = track_node.child("PositionXTimes");
		
		DDMap* ddMapX = new DDMap();
		
		for(pugi::xml_node ddtrack_node = positionXTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
		{
			std::string valIn, valOut;
			
			valIn = ddtrack_node.attribute("time").value();
			valOut = ddtrack_node.attribute("x").value();
			
			double in = atof(valIn.c_str());
			double out = atof(valOut.c_str());
			
			ddMapX->emplace(in,out);
		}
		
		data.time_value_map_x_ptr = ddMapX;
		
		
		pugi::xml_node positionYTimeNodeChild = track_node.child("PositionYTimes");
		
		DDMap* ddMapY = new DDMap();
		
		for(pugi::xml_node ddtrack_node = positionYTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
		{
			std::string valIn, valOut;
			
			valIn = ddtrack_node.attribute("time").value();
			valOut = ddtrack_node.attribute("y").value();
			
			double in = atof(valIn.c_str());
			double out = atof(valOut.c_str());
			
			ddMapY->emplace(in,out);
		}
		
		data.time_value_map_y_ptr = ddMapY;
		
		pugi::xml_node positionZTimeNodeChild = track_node.child("PositionZTimes");
		
		DDMap* ddMapZ = new DDMap();
		
		for(pugi::xml_node ddtrack_node = positionZTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
		{
			std::string valIn, valOut;
			
			valIn = ddtrack_node.attribute("time").value();
			valOut = ddtrack_node.attribute("z").value();
			
			double in = atof(valIn.c_str());
			double out = atof(valOut.c_str());
			
			ddMapZ->emplace(in,out);
		}
		
		data.time_value_map_z_ptr = ddMapZ;
		
		ptrSPTracksSaveDataVec->push_back(data);
	}
}

void XMLReader::LoadData_EchoZones(pugi::xml_node& root,std::vector <EchoZoneSaveData> *echoZonesSaveData)
{
	pugi::xml_node echoZonesNodeRoot = root.child("EffectZones").child("EchoZones");
			
	for(pugi::xml_node ez_node = echoZonesNodeRoot.first_child(); ez_node; ez_node = ez_node.next_sibling() )
	{
		EchoZoneSaveData data;
		
		std::string valString;
		
		valString = ez_node.child("Name").attribute("name").value();
		data.name = valString;
		
		
		valString = ez_node.child("Position").attribute("x").value();
		data.x = atof(valString.c_str());
		
		valString = ez_node.child("Position").attribute("y").value();
		data.y = atof(valString.c_str());
		
		valString = ez_node.child("Position").attribute("z").value();
		data.z = atof(valString.c_str());
		
		valString = ez_node.child("Size").attribute("width").value();
		data.width = atof(valString.c_str());
		
		valString = ez_node.child("Property").attribute("delay").value();
		data.properties.flDelay = atof(valString.c_str());
		
		valString = ez_node.child("Property").attribute("lrdelay").value();
		data.properties.flLRDelay = atof(valString.c_str());
		
		valString = ez_node.child("Property").attribute("damping").value();
		data.properties.flDamping = atof(valString.c_str());
		
		valString = ez_node.child("Property").attribute("feedback").value();
		data.properties.flFeedback = atof(valString.c_str());
		
		valString = ez_node.child("Property").attribute("spread").value();
		data.properties.flSpread = atof(valString.c_str());
		
		echoZonesSaveData->push_back(data);
	}
}

void XMLReader::LoadData_StandardRevZones(pugi::xml_node& root,std::vector <StandardReverbZoneSaveData> *standardRevZonesSaveData)
{
	pugi::xml_node standardRevZonesNodeRoot = root.child("EffectZones").child("StandardZones");
			
	for(pugi::xml_node srz_node = standardRevZonesNodeRoot.first_child(); srz_node; srz_node = srz_node.next_sibling() )
	{
		StandardReverbZoneSaveData data;
		
		std::string valString;
		
		valString = srz_node.child("Name").attribute("name").value();
		data.name = valString;
		
		
		valString = srz_node.child("Position").attribute("x").value();
		data.x = atof(valString.c_str());
		
		valString = srz_node.child("Position").attribute("y").value();
		data.y = atof(valString.c_str());
		
		valString = srz_node.child("Position").attribute("z").value();
		data.z = atof(valString.c_str());
		
		valString = srz_node.child("Size").attribute("width").value();
		data.width = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("density").value();
		data.properties.flDensity = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("diffusion").value();
		data.properties.flDiffusion = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("gain").value();
		data.properties.flGain = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("gainhf").value();
		data.properties.flGainHF = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("decaytime").value();
		data.properties.flDecayTime = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("decayhfratio").value();
		data.properties.flDecayHFRatio = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("reflectgain").value();
		data.properties.flReflectionsGain = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("reflectdelay").value();
		data.properties.flReflectionsDelay = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("latereverbgain").value();
		data.properties.flLateReverbGain = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("latereverbdelay").value();
		data.properties.flLateReverbDelay = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("airgainhf").value();
		data.properties.flAirAbsorptionGainHF = atof(valString.c_str());
		
		valString = srz_node.child("Property").attribute("roomrolloff").value();
		data.properties.flRoomRolloffFactor = atof(valString.c_str());
		
		standardRevZonesSaveData->push_back(data);
	}
}

void XMLReader::LoadData_EAXRevZones(pugi::xml_node& root,std::vector <EAXReverbZoneSaveData> *eaxRevZonesSaveData)
{
	pugi::xml_node eaxRevZonesNodeRoot = root.child("EffectZones").child("EAXZones");
			
	for(pugi::xml_node erz_node = eaxRevZonesNodeRoot.first_child(); erz_node; erz_node = erz_node.next_sibling() )
	{
		EAXReverbZoneSaveData data;
		
		std::string valString;
		
		valString = erz_node.child("Name").attribute("name").value();
		data.name = valString;
		
		
		valString = erz_node.child("Position").attribute("x").value();
		data.x = atof(valString.c_str());
		
		valString = erz_node.child("Position").attribute("y").value();
		data.y = atof(valString.c_str());
		
		valString = erz_node.child("Position").attribute("z").value();
		data.z = atof(valString.c_str());
		
		valString = erz_node.child("Size").attribute("width").value();
		data.width = atof(valString.c_str());
		
		
		valString = erz_node.child("Property").attribute("density").value();
		data.properties.flDensity = atof(valString.c_str()); 
		
		valString = erz_node.child("Property").attribute("diffusion").value();
		data.properties.flDiffusion = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("gain").value();
		data.properties.flGain = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("gainhf").value(); 
		data.properties.flGainHF = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("gainlf").value(); 
		data.properties.flGainLF = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("decaytime").value(); 
		data.properties.flDecayTime = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("decayhfratio").value(); 
		data.properties.flDecayHFRatio = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("decaylfratio").value(); 
		data.properties.flDecayLFRatio = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("reflectgain").value(); 
		data.properties.flReflectionsGain = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("reflectdelay").value(); 
		data.properties.flReflectionsDelay = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("latereverbgain").value(); 
		data.properties.flLateReverbGain = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("latereverbdelay").value(); 
		data.properties.flLateReverbDelay = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("echotime").value(); 
		data.properties.flEchoTime = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("echodepth").value(); 
		data.properties.flEchoDepth = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("modtime").value(); 
		data.properties.flModulationTime = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("moddepth").value(); 
		data.properties.flModulationDepth = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("hfref").value(); 
		data.properties.flHFReference = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("lfref").value(); 
		data.properties.flLFReference = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("airgainhf").value(); 
		data.properties.flAirAbsorptionGainHF = atof(valString.c_str());
		
		valString = erz_node.child("Property").attribute("roomrolloff").value(); 
		data.properties.flRoomRolloffFactor = atof(valString.c_str());
		
		eaxRevZonesSaveData->push_back(data);
	}
}

void XMLReader::LoadData_ListenerTrack(pugi::xml_node& root, ListenerTrackSaveData& listener_track_data)
{
	pugi::xml_node ltTrackRoot = root.child("ListenerTrack");
	
	pugi::xml_node track_node = ltTrackRoot.first_child();
		
	ListenerTrackSaveData data;
	std::string valString;
	
	
	pugi::xml_node positionXTimeNodeChild = track_node.child("PositionXTimes");
	
	DDMap* ddMapX = new DDMap();
	
	for(pugi::xml_node ddtrack_node = positionXTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
	{
		std::string valIn, valOut;
		
		valIn = ddtrack_node.attribute("time").value();
		valOut = ddtrack_node.attribute("x").value();
		
		double in = atof(valIn.c_str());
		double out = atof(valOut.c_str());
		
		ddMapX->emplace(in,out);
	}
	
	data.time_value_map_posx_ptr = ddMapX;
	
	
	pugi::xml_node positionYTimeNodeChild = track_node.child("PositionYTimes");
	
	DDMap* ddMapY = new DDMap();
	
	for(pugi::xml_node ddtrack_node = positionYTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
	{
		std::string valIn, valOut;
		
		valIn = ddtrack_node.attribute("time").value();
		valOut = ddtrack_node.attribute("y").value();
		
		double in = atof(valIn.c_str());
		double out = atof(valOut.c_str());
		
		ddMapY->emplace(in,out);
	}
	
	data.time_value_map_posy_ptr = ddMapY;
	
	pugi::xml_node positionZTimeNodeChild = track_node.child("PositionZTimes");
	
	DDMap* ddMapZ = new DDMap();
	
	for(pugi::xml_node ddtrack_node = positionZTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
	{
		std::string valIn, valOut;
		
		valIn = ddtrack_node.attribute("time").value();
		valOut = ddtrack_node.attribute("z").value();
		
		double in = atof(valIn.c_str());
		double out = atof(valOut.c_str());
		
		ddMapZ->emplace(in,out);
	}
	
	data.time_value_map_posz_ptr = ddMapZ;
	
	
	
	DDMap* ddMapQuatW = new DDMap();
	
	pugi::xml_node orientationWTimeNodeChild = track_node.child("OrientationWTimes");
	
	for(pugi::xml_node ddtrack_node = orientationWTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
	{
		std::string valIn, valOut;
		
		valIn = ddtrack_node.attribute("time").value();
		valOut = ddtrack_node.attribute("qw").value();
		
		double in = atof(valIn.c_str());
		double out = atof(valOut.c_str());
		
		ddMapQuatW->emplace(in,out);
	}
	
	data.time_value_map_orientw_ptr = ddMapQuatW;
	
	
	
	DDMap* ddMapQuatX = new DDMap();
	
	pugi::xml_node orientationXTimeNodeChild = track_node.child("OrientationXTimes");
	
	for(pugi::xml_node ddtrack_node = orientationXTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
	{
		std::string valIn, valOut;
		
		valIn = ddtrack_node.attribute("time").value();
		valOut = ddtrack_node.attribute("qx").value();
		
		double in = atof(valIn.c_str());
		double out = atof(valOut.c_str());
		
		ddMapQuatX->emplace(in,out);
	}
	
	data.time_value_map_orientx_ptr = ddMapQuatX;
	
	
	
	DDMap* ddMapQuatY = new DDMap();
	
	pugi::xml_node orientationYTimeNodeChild = track_node.child("OrientationYTimes");
	
	for(pugi::xml_node ddtrack_node = orientationYTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
	{
		std::string valIn, valOut;
		
		valIn = ddtrack_node.attribute("time").value();
		valOut = ddtrack_node.attribute("qy").value();
		
		double in = atof(valIn.c_str());
		double out = atof(valOut.c_str());
		
		ddMapQuatY->emplace(in,out);
	}
	
	data.time_value_map_orienty_ptr = ddMapQuatY;
	
	
	
	DDMap* ddMapQuatZ = new DDMap();
	
	pugi::xml_node orientationZTimeNodeChild = track_node.child("OrientationZTimes");
	
	for(pugi::xml_node ddtrack_node = orientationZTimeNodeChild.first_child(); ddtrack_node; ddtrack_node = ddtrack_node.next_sibling() )
	{
		std::string valIn, valOut;
		
		valIn = ddtrack_node.attribute("time").value();
		valOut = ddtrack_node.attribute("qz").value();
		
		double in = atof(valIn.c_str());
		double out = atof(valOut.c_str());
		
		ddMapQuatZ->emplace(in,out);
	}
	
	data.time_value_map_orientz_ptr = ddMapQuatZ;
	
	listener_track_data = data;
}

void XMLReader::LoadData_Listener(pugi::xml_node& root, ListenerSaveData& listener_save_data)
{
	ListenerSaveData data;
	
	pugi::xml_node listenerNodeRoot = root.child("Listener");
			
	std::string valString = "";
	bool status = false;
	
	valString = listenerNodeRoot.child("FreeRoam").attribute("status").value();
	
	if(valString == "true"){status = true;}
	else{status = false;}
	
	data.freeRoam = status;
	
	valString = "";
	valString = listenerNodeRoot.child("ExternalOrientation").attribute("status").value();
	
	if(valString == "true"){status = true;}
	else{status = false;}
	
	data.externalOrientation = status;
	
	listener_save_data = data;
}
