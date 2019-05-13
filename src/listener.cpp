#include "listener.h"

Listener::Listener()
{
	//initialize listener position
    listener_position_vector.resize(3);

    //initialize listener orientation
    listener_orientation_vector.resize(6);
    
    Listener::initListener();   

}

Listener::~Listener()
{
	//std::cout << "Listener destructor called! \n";
}

void Listener::initListener()
{
	std::cout << "Init Listener called! \n";
	
	    //set listener position at origin
	listener_position_vector[POSITION_INDEX::X] = 0.0f;
	listener_position_vector[POSITION_INDEX::Y] = 0.0f;
	listener_position_vector[POSITION_INDEX::Z] = 0.0f;
	
	//std::cout << "Listener x:" << Listener::getPositionX();
	//std::cout << "\nListener y:" << Listener::getPositionY();
	//std::cout << "\nListener z:" << Listener::getPositionZ();
	
	//Set Listener orientation
	
	//set to just facing the screen

	//set where head is facing
	listener_orientation_vector[ORIENTATION_INDEX::FORWARD_X] = 0.0f; //forward vector x value
	listener_orientation_vector[ORIENTATION_INDEX::FORWARD_Y] = 0.0f; //forward vector y value
	listener_orientation_vector[ORIENTATION_INDEX::FORWARD_Z] = -1.0f; //forward vector z value
	//set direction of top of head surface vector
	listener_orientation_vector[ORIENTATION_INDEX::UP_X] = 0.0f; //up vector x value
	listener_orientation_vector[ORIENTATION_INDEX::UP_Y] = 1.0f; //up vector y value
	listener_orientation_vector[ORIENTATION_INDEX::UP_Z] = 0.0f; //up vector z value
		
	//set current listener orientation
	alListenerfv(AL_ORIENTATION, listener_orientation_vector.data());
	
	//Initialize Listener position	
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);//is not moving in 3d space
	
		//make box
	//create ShapeDrawable object
	m_renderObject= new osg::ShapeDrawable;
	m_box = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),1.0f);

	//make ShapeDrawable object a box 
	//initialize box at certain position 
	m_renderObject->setShape( m_box );
	//set color of ShapeDrawable object with box
	m_renderObject->setColor( osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) );
	
	//add ShapeDrawable box to geometry root node
	m_geode = new osg::Geode;
	m_geode->addDrawable( m_renderObject.get() );
	
	//add geode to position attitude transform
	m_paTransform = new osg::PositionAttitudeTransform;
	m_paTransform->setPosition( osg::Vec3(
											listener_position_vector[POSITION_INDEX::X],
											listener_position_vector[POSITION_INDEX::Y],
											listener_position_vector[POSITION_INDEX::Z]));
	m_paTransform->addChild(m_geode);
}

void Listener::setListenerPosition()
{
	
	alListener3f(AL_POSITION, 
					listener_position_vector[POSITION_INDEX::X], 
					listener_position_vector[POSITION_INDEX::Y], 
					listener_position_vector[POSITION_INDEX::Z]);

	m_paTransform->setPosition(osg::Vec3(
								listener_position_vector[POSITION_INDEX::Y], 
								listener_position_vector[POSITION_INDEX::Y], 
								listener_position_vector[POSITION_INDEX::Z])
								);
}

void Listener::setPositionX(float& x)
{		
	listener_position_vector[POSITION_INDEX::X] = x;
	
	Listener::setListenerPosition();
} 

float Listener::getPositionX(){return listener_position_vector[POSITION_INDEX::X];} 

void Listener::setPositionY(float& y)
{
	listener_position_vector[POSITION_INDEX::Y] = y;
	
	Listener::setListenerPosition();
} 

float Listener::getPositionY(){return listener_position_vector[POSITION_INDEX::Y];}

void Listener::setPositionZ(float& z)
{
	listener_position_vector[POSITION_INDEX::Z] = z;

	Listener::setListenerPosition();	
}
 
float Listener::getPositionZ(){return listener_position_vector[POSITION_INDEX::Z];}

//Listener Orientation Functions

void Listener::setListenerOrientation()
{
	alListenerfv(AL_ORIENTATION, listener_orientation_vector.data());//change OpenAL Soft internal listener orientation
}

void Listener::setForwardX(float& x)
{   
	listener_orientation_vector[ORIENTATION_INDEX::FORWARD_X] = x;
	Listener::setListenerOrientation();

}

float Listener::getForwardX(){return listener_orientation_vector[ORIENTATION_INDEX::FORWARD_X];}

void Listener::setForwardY(float& y)
{
	listener_orientation_vector[ORIENTATION_INDEX::FORWARD_Y] = y;
	Listener::setListenerOrientation();
    
}

float Listener::getForwardY(){return listener_orientation_vector[ORIENTATION_INDEX::FORWARD_Y];}

void Listener::setForwardZ(float& z)
{
	listener_orientation_vector[ORIENTATION_INDEX::FORWARD_Z] = z;
	Listener::setListenerOrientation();
}

float Listener::getForwardZ(){return listener_orientation_vector[ORIENTATION_INDEX::FORWARD_Z];}

void Listener::setUpX(float& x)
{
	listener_orientation_vector[ORIENTATION_INDEX::UP_X] = x;
	Listener::setListenerOrientation();
}

float Listener::getUpX(){return listener_orientation_vector[ORIENTATION_INDEX::UP_X];}

void Listener::setUpY(float& y)
{
	listener_orientation_vector[ORIENTATION_INDEX::UP_Y] = y;
	Listener::setListenerOrientation();   
}

float Listener::getUpY(){return listener_orientation_vector[ORIENTATION_INDEX::UP_Y];}

void Listener::setUpZ(float& z)
{
	listener_orientation_vector[ORIENTATION_INDEX::UP_Z] = z;
	Listener::setListenerOrientation();
}

float Listener::getUpZ(){return listener_orientation_vector[ORIENTATION_INDEX::UP_Z];}

osg::ShapeDrawable* Listener::getRenderObject(){return m_renderObject;}

osg::Geode* Listener::getGeodeNode(){return m_geode;}

osg::PositionAttitudeTransform* Listener::getTransformNode(){return m_paTransform;}
