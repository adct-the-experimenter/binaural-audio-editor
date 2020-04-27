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

#include "effect-zone.h"

/*
 * Some code was taken from OpenAL Reverb Example
 * OpenAL Reverb Example
 *
 * Copyright (c) 2012 by Chris Robinson <chris.kcat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
/* Effect object functions */
static LPALGENEFFECTS alGenEffects;
static LPALDELETEEFFECTS alDeleteEffects;
static LPALISEFFECT alIsEffect;
static LPALEFFECTI alEffecti;
static LPALEFFECTIV alEffectiv;
static LPALEFFECTF alEffectf;
static LPALEFFECTFV alEffectfv;
static LPALGETEFFECTI alGetEffecti;
static LPALGETEFFECTIV alGetEffectiv;
static LPALGETEFFECTF alGetEffectf;
static LPALGETEFFECTFV alGetEffectfv;

/* Auxiliary Effect Slot object functions */
static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;


EffectZone::EffectZone()
{	
	
	//initialize position vector
	position_vector.resize(3);
	position_vector[POSITION_INDEX::X] = 0;
	position_vector[POSITION_INDEX::Y] = 0;
	position_vector[POSITION_INDEX::Z] = 0;
	
	
}

EffectZone::~EffectZone()
{
	
}

void EffectZone::InitEffectZone(std::string& thisName,
									double& x, double& y, double& z, double& width)
{   
	//set name
	name = thisName;
	
	//set position
	position_vector[POSITION_INDEX::X] = x;
	position_vector[POSITION_INDEX::Y] = y;
	position_vector[POSITION_INDEX::Z] = z;
	
	//set width
	m_width = width;
	
}

void EffectZone::InitEffectZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width, ZoneColor& color)
{
	
	EffectZone::InitEffectZone( thisName,x, y, z, width);
	
	//make box
	//create ShapeDrawable object
	m_renderObject = new osg::ShapeDrawable;
	m_box = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),m_width);

	//make ShapeDrawable object a box
	//initialize box at certain position
	m_renderObject->setShape(m_box);
	//set color of ShapeDrawable object with box
	m_renderObject->setColor( osg::Vec4(color.r, color.g, color.b, color.alpha) );

	m_geode = new osg::Geode;
	m_geode->addDrawable( m_renderObject.get() );
	
	//make transparent
	osg::StateSet* ss = new osg::StateSet();
	m_geode->setStateSet(ss);
	m_geode->getStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
	m_geode->getStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	// Create transformation node
	m_paTransform = new osg::PositionAttitudeTransform;

	//initialize transform and add geode to it
	m_paTransform->setPosition( osg::Vec3(x,y,z));
	m_paTransform->addChild(m_geode);
}



void EffectZone::SetNameString(std::string& thisName){ name = thisName;}
std::string EffectZone::GetNameString(){ return name;}


void EffectZone::SetPositionX(double& x)
{
	position_vector[POSITION_INDEX::X] = x;

	m_paTransform->setPosition(osg::Vec3(x,
								position_vector[POSITION_INDEX::Y],
								position_vector[POSITION_INDEX::Z]));
}

double EffectZone::GetPositionX(){return position_vector[POSITION_INDEX::X];}

void EffectZone::SetPositionY(double& y)
{
	position_vector[POSITION_INDEX::Y] = y;

	m_paTransform->setPosition(osg::Vec3(position_vector[POSITION_INDEX::X],
								y,
								position_vector[POSITION_INDEX::Z]));
}

double EffectZone::GetPositionY(){return position_vector[POSITION_INDEX::Y];}

void EffectZone::SetPositionZ(double& z)
{
	position_vector[POSITION_INDEX::Z] = z;

	m_paTransform->setPosition(osg::Vec3(position_vector[POSITION_INDEX::X],
								position_vector[POSITION_INDEX::Y],
								z));
}

double EffectZone::GetPositionZ(){return position_vector[POSITION_INDEX::Z];}


osg::ShapeDrawable* EffectZone::getRenderObject(){return m_renderObject;}

osg::Geode* EffectZone::getGeodeNode(){return m_geode;}

osg::PositionAttitudeTransform* EffectZone::getTransformNode(){return m_paTransform;}



void EffectZone::ChangeWidth(double width)
{
	
	m_width = width;
	
	//remove drawable of box from geode 
	m_geode->removeDrawable( m_renderObject.get() );
	
	
	//make box with new width
	//create ShapeDrawable object
	m_renderObject = new osg::ShapeDrawable;
	m_box = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),m_width);

	//make ShapeDrawable object a box
	//initialize box at certain position
	m_renderObject->setShape(m_box);
	
	//set color of ShapeDrawable object with box
	m_renderObject->setColor( osg::Vec4(m_color.r, m_color.g, m_color.b, m_color.alpha) );
	
	//add new drawable to geode
	m_geode->addDrawable( m_renderObject.get() );
	
}

double EffectZone::GetWidth(){return m_width;}

void EffectZone::SetColor(ZoneColor color)
{
	m_color = color;
	//set color of ShapeDrawable object with box
	if(m_renderObject)
	{
		m_renderObject->setColor( osg::Vec4(color.r,color.g, color.b, color.alpha) );
	}
}

void EffectZone::SetEffectsSlotPointer(ALuint* slot_ptr){m_slot_ptr = slot_ptr;}

ALuint* EffectZone::GetEffectPointer(){return nullptr;}
ALuint* EffectZone::GetEffectsSlotPointer(){ std::cout << "Base class called! slot ptr:" << m_slot_ptr << std::endl; return m_slot_ptr;}
	
ALuint EffectZone::GetEffect(){return 0;}
ALuint EffectZone::GetEffectsSlot(){return 0;}
	
void EffectZone::FreeEffects(){}


