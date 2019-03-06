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

void SoundProducer::setListenerPositionX(float x){producer_position_vector[POSITION_INDEX::X] = x;} 
float SoundProducer::getListenerPositionX(){return producer_position_vector[POSITION_INDEX::X];} 
void SoundProducer::setListenerPositionY(float y){producer_position_vector[POSITION_INDEX::Y] = y;} 
float SoundProducer::getListenerPositionY(){return producer_position_vector[POSITION_INDEX::Y];}
void SoundProducer::setListenerPositionZ(float z){producer_position_vector[POSITION_INDEX::Z] = z;} 
float SoundProducer::getListenerPositionZ(){return producer_position_vector[POSITION_INDEX::Z];}

void SoundProducer::setFilepathToSound(std::string& filepath){m_filepath = filepath;}

std::string& SoundProducer::getFilepathToSound(){return m_filepath;}

ALuint* SoundProducer::getBuffer(){return &m_buffer;}

ALuint* SoundProducer::getSource(){return & m_source;}
