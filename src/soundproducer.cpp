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
		alDeleteSources(1, &m_source);
	}
	
	if(m_buffer != 0)
	{
		alDeleteBuffers(1, &m_buffer);
	}
}

void SoundProducer::InitSoundProducer(double& x, double& y, double& z)
{
	producer_position_vector[POSITION_INDEX::X] = x;
	producer_position_vector[POSITION_INDEX::Y] = y;
	producer_position_vector[POSITION_INDEX::Z] = z; 
	
	//make box
	//create ShapeDrawable object
	renderObject = new osg::ShapeDrawable;
	box = new osg::Box(osg::Vec3(x, y, z),1.0f);
	
	//make ShapeDrawable object a box 
	//initialize box at certain position 
	renderObject->setShape(box);
	//set color of ShapeDrawable object with box
	renderObject->setColor( osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f) );
	
}

void SoundProducer::setPositionX(double& x)
{
	producer_position_vector[POSITION_INDEX::X] = x;
	box->setCenter(osg::Vec3(x, 
							producer_position_vector[POSITION_INDEX::Y],
							producer_position_vector[POSITION_INDEX::Z]	
							)
				   );
	
} 

float SoundProducer::getPositionX(){return producer_position_vector[POSITION_INDEX::X];} 

void SoundProducer::setPositionY(double& y)
{
	producer_position_vector[POSITION_INDEX::Y] = y;
	
	box->setCenter(osg::Vec3(producer_position_vector[POSITION_INDEX::X], 
							y,
							producer_position_vector[POSITION_INDEX::Z]	
							)
				   );
} 

float SoundProducer::getPositionY(){return producer_position_vector[POSITION_INDEX::Y];}

void SoundProducer::setPositionZ(double& z)
{
	producer_position_vector[POSITION_INDEX::Z] = z;
	
	box->setCenter(osg::Vec3(producer_position_vector[POSITION_INDEX::X], 
							producer_position_vector[POSITION_INDEX::Y],
							z	
							)
				   );
} 
float SoundProducer::getPositionZ(){return producer_position_vector[POSITION_INDEX::Z];}

void SoundProducer::setFilepathToSound(std::string& filepath){m_filepath = filepath;}

std::string& SoundProducer::getFilepathToSound(){return m_filepath;}

ALuint* SoundProducer::getBuffer(){return &m_buffer;}

ALuint* SoundProducer::getSource(){return &m_source;}

osg::ShapeDrawable* SoundProducer::getRenderObject(){return renderObject;}
