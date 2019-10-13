// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifdef WIN32
#include <winsock2.h>
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "reverb-zone.h"

ReverbZone::ReverbZone()
{

	//initialize position vector
	producer_position_vector.resize(3);
	producer_position_vector[POSITION_INDEX::X] = 0;
	producer_position_vector[POSITION_INDEX::Y] = 0;
	producer_position_vector[POSITION_INDEX::Z] = 0;

}

ReverbZone::~ReverbZone()
{
	
}

void ReverbZone::InitReverbZone(std::string& thisName,
									double& x, double& y, double& z, double& width, double& height)
{

	name = thisName;

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
	
	moveZone();
}

void ReverbZone::SetNameString(std::string& thisName){ name = thisName;}
std::string ReverbZone::GetNameString(){ return name;}


void ReverbZone::SetPositionX(double& x)
{
	producer_position_vector[POSITION_INDEX::X] = x;

	m_paTransform->setPosition(osg::Vec3(x,
								producer_position_vector[POSITION_INDEX::Y],
								producer_position_vector[POSITION_INDEX::Z]));
	moveZone();
}

double ReverbZone::GetPositionX(){return producer_position_vector[POSITION_INDEX::X];}

void ReverbZone::SetPositionY(double& y)
{
	producer_position_vector[POSITION_INDEX::Y] = y;

	m_paTransform->setPosition(osg::Vec3(producer_position_vector[POSITION_INDEX::X],
								y,
								producer_position_vector[POSITION_INDEX::Z]));
	moveZone();
}

double ReverbZone::GetPositionY(){return producer_position_vector[POSITION_INDEX::Y];}

void ReverbZone::SetPositionZ(double& z)
{
	producer_position_vector[POSITION_INDEX::Z] = z;

	m_paTransform->setPosition(osg::Vec3(producer_position_vector[POSITION_INDEX::X],
								producer_position_vector[POSITION_INDEX::Y],
								z));
	moveZone();
}

double ReverbZone::GetPositionZ(){return producer_position_vector[POSITION_INDEX::Z];}

void ReverbZone::moveZone()
{
	
}

osg::ShapeDrawable* ReverbZone::getRenderObject(){return m_renderObject;}

osg::Geode* ReverbZone::getGeodeNode(){return m_geode;}

osg::PositionAttitudeTransform* ReverbZone::getTransformNode(){return m_paTransform;}

