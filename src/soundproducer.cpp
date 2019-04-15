#include "soundproducer.h"

SoundProducer::SoundProducer()
{
	m_buffer = 0;
	m_source = 0;
	
	producer_position_vector.resize(3);
	producer_position_vector[POSITION_INDEX::X] = 0;
	producer_position_vector[POSITION_INDEX::Y] = 0;
	producer_position_vector[POSITION_INDEX::Z] = 0;
	
	rootGeode = new osg::Geode;
	
	// Create transformation node
	osg::ref_ptr<osg::PositionAttitudeTransform> myPat = new osg::PositionAttitudeTransform;

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

void SoundProducer::InitSoundProducer(std::string& thisName,double& x, double& y, double& z)
{
	name = thisName;
	
	//set position
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
	
	rootGeode->addDrawable( renderObject.get() );
	
}

void SoundProducer::SetNameString(std::string& thisName){ name = thisName;}
std::string SoundProducer::GetNameString(){ return name;}

void SoundProducer::setPositionX(double& x)
{
	std::cout << "\nSet Position X called! \n";
	
		
	producer_position_vector[POSITION_INDEX::X] = x;
	
	myPat->setPosition(osg::Vec3(x, 
								producer_position_vector[POSITION_INDEX::Y], 
								producer_position_vector[POSITION_INDEX::Z]));

	// Attach node to be transformed
	myPat->addChild(rootGeode.get());
	
} 

float SoundProducer::getPositionX(){return producer_position_vector[POSITION_INDEX::X];} 

void SoundProducer::setPositionY(double& y)
{
	producer_position_vector[POSITION_INDEX::Y] = y;
	
	//box->setCenter(osg::Vec3(producer_position_vector[POSITION_INDEX::X], 
	//						y,
	//						producer_position_vector[POSITION_INDEX::Z]	
	//						)
	//			   );
} 

float SoundProducer::getPositionY(){return producer_position_vector[POSITION_INDEX::Y];}

void SoundProducer::setPositionZ(double& z)
{
	producer_position_vector[POSITION_INDEX::Z] = z;
	
	//box->setCenter(osg::Vec3(producer_position_vector[POSITION_INDEX::X], 
	//						producer_position_vector[POSITION_INDEX::Y],
	//						z	
	//						)
	//			   );
} 
float SoundProducer::getPositionZ(){return producer_position_vector[POSITION_INDEX::Z];}

void SoundProducer::setFilepathToSound(std::string& filepath){m_filepath = filepath;}

std::string& SoundProducer::getFilepathToSound(){return m_filepath;}

ALuint* SoundProducer::getBuffer(){return &m_buffer;}

ALuint* SoundProducer::getSource(){return &m_source;}

osg::ShapeDrawable* SoundProducer::getRenderObject(){return renderObject;}

osg::Geode* SoundProducer::getRootGeodeNode(){return rootGeode;}

osg::PositionAttitudeTransform* SoundProducer::getTransformNode(){return myPat;}
