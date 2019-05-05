#include "soundproducer.h"

SoundProducer::SoundProducer()
{
	m_buffer = 0;
	m_source = 0;
	
	producer_position_vector.resize(3);
	producer_position_vector[POSITION_INDEX::X] = 0;
	producer_position_vector[POSITION_INDEX::Y] = 0;
	producer_position_vector[POSITION_INDEX::Z] = 0;
	

}

SoundProducer::~SoundProducer()
{
	if(m_source != 0)
	{
		#ifdef _cplusplus
		//being compiled by a c++ compulter, inhibit mangling
		extern "C" 
		{
			alDeleteSources(1, &m_source);
		}
		#endif
	}
	
	if(m_buffer != 0)
	{
		#ifdef _cplusplus
		//being compiled by a c++ compulter, inhibit mangling
		extern "C" 
		{
			alDeleteBuffers(1, &m_buffer);
		}
		#endif
	}
}

void SoundProducer::InitSoundProducer(std::string& thisName,std::string& filepath, ALuint& buffer,
									double& x, double& y, double& z)
{
	name = thisName;
	m_filepath = filepath;
	
	m_buffer = buffer;
	
	//set position
	producer_position_vector[POSITION_INDEX::X] = x;
	producer_position_vector[POSITION_INDEX::Y] = y;
	producer_position_vector[POSITION_INDEX::Z] = z; 
	
	//make box
	//create ShapeDrawable object
	m_renderObject = new osg::ShapeDrawable;
	m_box = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),1.0f);
	
	//make ShapeDrawable object a box 
	//initialize box at certain position 
	m_renderObject->setShape(m_box);
	//set color of ShapeDrawable object with box
	m_renderObject->setColor( osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f) );
	
	m_geode = new osg::Geode;
	m_geode->addDrawable( m_renderObject.get() );
	
	
	// Create transformation node
	m_paTransform = new osg::PositionAttitudeTransform;
	
	//initialize transform and add geode to it
	m_paTransform->setPosition( osg::Vec3(x,y,z));
	m_paTransform->addChild(m_geode);
	
	moveSource();
}

void SoundProducer::SetNameString(std::string& thisName){ name = thisName;}
std::string SoundProducer::GetNameString(){ return name;}

void SoundProducer::moveSource()
{
	//if source is defined
	if(m_source != 0)
	{
		#ifdef _cplusplus
		//being compiled by a c++ compulter, inhibit mangling
		extern "C" 
		{
			//move source 
			alSource3f(m_source, AL_POSITION, 
					(ALfloat)producer_position_vector[POSITION_INDEX::X], 
					(ALfloat)producer_position_vector[POSITION_INDEX::Y], 
					(ALfloat)producer_position_vector[POSITION_INDEX::Z]);
		}
		#endif
	}
}

void SoundProducer::setPositionX(double& x)
{		
	producer_position_vector[POSITION_INDEX::X] = x;
	
	m_paTransform->setPosition(osg::Vec3(x, 
								producer_position_vector[POSITION_INDEX::Y], 
								producer_position_vector[POSITION_INDEX::Z]));
	moveSource();
} 

float SoundProducer::getPositionX(){return producer_position_vector[POSITION_INDEX::X];} 

void SoundProducer::setPositionY(double& y)
{
	producer_position_vector[POSITION_INDEX::Y] = y;
	
	m_paTransform->setPosition(osg::Vec3(producer_position_vector[POSITION_INDEX::X], 
								y, 
								producer_position_vector[POSITION_INDEX::Z]));
	moveSource();
} 

float SoundProducer::getPositionY(){return producer_position_vector[POSITION_INDEX::Y];}

void SoundProducer::setPositionZ(double& z)
{
	producer_position_vector[POSITION_INDEX::Z] = z;
	
	m_paTransform->setPosition(osg::Vec3(producer_position_vector[POSITION_INDEX::X], 
								producer_position_vector[POSITION_INDEX::Y], 
								z));
	moveSource();
}
 
float SoundProducer::getPositionZ(){return producer_position_vector[POSITION_INDEX::Z];}

void SoundProducer::setFilepathToSound(std::string& filepath){m_filepath = filepath;}

std::string& SoundProducer::getFilepathToSound(){return m_filepath;}

void SoundProducer::setBuffer(ALuint& thisBuffer){m_buffer = thisBuffer;}
ALuint* SoundProducer::getBuffer(){return &m_buffer;}

void SoundProducer::setSource(ALuint& thisSource){m_source = thisSource;}
ALuint* SoundProducer::getSource(){return &m_source;}

osg::ShapeDrawable* SoundProducer::getRenderObject(){return m_renderObject;}

osg::Geode* SoundProducer::getGeodeNode(){return m_geode;}

osg::PositionAttitudeTransform* SoundProducer::getTransformNode(){return m_paTransform;}
