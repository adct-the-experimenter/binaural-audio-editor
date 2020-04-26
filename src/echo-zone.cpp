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

#include "echo-zone.h"

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


EchoZone::EchoZone()
{	
	//initialize OpenAL soft effects properties
	
	m_effect = 0;
	m_slot = 0;
	
	//initialize position vector
	position_vector.resize(3);
	position_vector[POSITION_INDEX::X] = 0;
	position_vector[POSITION_INDEX::Y] = 0;
	position_vector[POSITION_INDEX::Z] = 0;
	
	
}

EchoZone::~EchoZone()
{
	if(m_effect != 0 && m_slot != 0)
	{	
		FreeEffects();
	}
}

void EchoZone::InitEchoZone(std::string& thisName,
									double& x, double& y, double& z, double& width,
									EchoZoneProperties& properties)
{
	#define LOAD_PROC(T, x)  ((x) = (T)alGetProcAddress(#x))
		
		LOAD_PROC(LPALDELETEEFFECTS, alDeleteEffects);
		LOAD_PROC(LPALISEFFECT, alIsEffect);
		LOAD_PROC(LPALEFFECTI, alEffecti);
		LOAD_PROC(LPALEFFECTIV, alEffectiv);
		LOAD_PROC(LPALEFFECTF, alEffectf);
		LOAD_PROC(LPALEFFECTFV, alEffectfv);
		LOAD_PROC(LPALGETEFFECTI, alGetEffecti);
		LOAD_PROC(LPALGETEFFECTIV, alGetEffectiv);
		LOAD_PROC(LPALGETEFFECTF, alGetEffectf);
		LOAD_PROC(LPALGETEFFECTFV, alGetEffectfv);

		LOAD_PROC(LPALGENAUXILIARYEFFECTSLOTS, alGenAuxiliaryEffectSlots);
		LOAD_PROC(LPALDELETEAUXILIARYEFFECTSLOTS, alDeleteAuxiliaryEffectSlots);
		LOAD_PROC(LPALISAUXILIARYEFFECTSLOT, alIsAuxiliaryEffectSlot);
		LOAD_PROC(LPALAUXILIARYEFFECTSLOTI, alAuxiliaryEffectSloti);
		LOAD_PROC(LPALAUXILIARYEFFECTSLOTIV, alAuxiliaryEffectSlotiv);
		LOAD_PROC(LPALAUXILIARYEFFECTSLOTF, alAuxiliaryEffectSlotf);
		LOAD_PROC(LPALAUXILIARYEFFECTSLOTFV, alAuxiliaryEffectSlotfv);
		LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTI, alGetAuxiliaryEffectSloti);
		LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTIV, alGetAuxiliaryEffectSlotiv);
		LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTF, alGetAuxiliaryEffectSlotf);
		LOAD_PROC(LPALGETAUXILIARYEFFECTSLOTFV, alGetAuxiliaryEffectSlotfv);
	#undef LOAD_PROC
	
	
	
	/* Create the effect object. */
	m_effect = 0;
	alGenEffects(1, &m_effect);
	
	alEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_ECHO);

	alEffectf(m_effect, AL_ECHO_DELAY, properties.flDelay);
	alEffectf(m_effect, AL_ECHO_LRDELAY, properties.flLRDelay);
	alEffectf(m_effect, AL_ECHO_DAMPING, properties.flDamping);
	alEffectf(m_effect, AL_ECHO_FEEDBACK, properties.flFeedback);
	alEffectf(m_effect, AL_ECHO_SPREAD, properties.flSpread);
	
	/* Create the effect slot object. This is what "plays" an effect on sources
     * that connect to it. */
    m_slot = 0;
    alGenAuxiliaryEffectSlots(1, &m_slot);

    /* Tell the effect slot to use the loaded effect object. Note that the this
     * effectively copies the effect properties. You can modify or delete the
     * effect object afterward without affecting the effect slot.
     */
    alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, (ALint)m_effect);
    assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
    
	//set name
	name = thisName;
	
	//set position
	position_vector[POSITION_INDEX::X] = x;
	position_vector[POSITION_INDEX::Y] = y;
	position_vector[POSITION_INDEX::Z] = z;
	
	//set width
	m_width = width;
	
	//initialize standard properties
	m_echo_prop = properties;
	
}

void EchoZone::InitEchoZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width,
							EchoZoneProperties& properties)
{
	
	EchoZone::InitEchoZone( thisName,
									x, y, z, width,
									properties);
	
	//make box
	//create ShapeDrawable object
	m_renderObject = new osg::ShapeDrawable;
	m_box = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),m_width);

	//make ShapeDrawable object a box
	//initialize box at certain position
	m_renderObject->setShape(m_box);
	//set color of ShapeDrawable object with box
	m_renderObject->setColor( osg::Vec4(0.4f, 0.3f, 0.0f, 0.3f) );

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

void EchoZone::ChangeEchoZoneProperties(EchoZoneProperties& properties)
{

	/* Set new porperties */
	alEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_ECHO);

	alEffectf(m_effect, AL_ECHO_DELAY, properties.flDelay);
	alEffectf(m_effect, AL_ECHO_LRDELAY, properties.flLRDelay);
	alEffectf(m_effect, AL_ECHO_DAMPING, properties.flDamping);
	alEffectf(m_effect, AL_ECHO_FEEDBACK, properties.flFeedback);
	alEffectf(m_effect, AL_ECHO_SPREAD, properties.flSpread);
	
    
    m_echo_prop = properties;
    
    //load the newly modified effect into the effect slot
	
	/* Tell the effect slot to use the loaded effect object. Note that the this
     * effectively copies the effect properties. You can modify or delete the
     * effect object afterward without affecting the effect slot.
     */
    alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, (ALint)m_effect);
    assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
}


EchoZoneProperties& EchoZone::GetEchoZoneProperties()
{
	return m_echo_prop;
}


void EchoZone::SetNameString(std::string& thisName){ name = thisName;}
std::string EchoZone::GetNameString(){ return name;}


void EchoZone::SetPositionX(double& x)
{
	position_vector[POSITION_INDEX::X] = x;

	m_paTransform->setPosition(osg::Vec3(x,
								position_vector[POSITION_INDEX::Y],
								position_vector[POSITION_INDEX::Z]));
}

double EchoZone::GetPositionX(){return position_vector[POSITION_INDEX::X];}

void EchoZone::SetPositionY(double& y)
{
	position_vector[POSITION_INDEX::Y] = y;

	m_paTransform->setPosition(osg::Vec3(position_vector[POSITION_INDEX::X],
								y,
								position_vector[POSITION_INDEX::Z]));
}

double EchoZone::GetPositionY(){return position_vector[POSITION_INDEX::Y];}

void EchoZone::SetPositionZ(double& z)
{
	position_vector[POSITION_INDEX::Z] = z;

	m_paTransform->setPosition(osg::Vec3(position_vector[POSITION_INDEX::X],
								position_vector[POSITION_INDEX::Y],
								z));
}

double EchoZone::GetPositionZ(){return position_vector[POSITION_INDEX::Z];}


osg::ShapeDrawable* EchoZone::getRenderObject(){return m_renderObject;}

osg::Geode* EchoZone::getGeodeNode(){return m_geode;}

osg::PositionAttitudeTransform* EchoZone::getTransformNode(){return m_paTransform;}


ALuint* EchoZone::GetEffect(){return &m_effect;}
ALuint* EchoZone::GetEffectsSlot(){return &m_slot;}

void EchoZone::FreeEffects()
{
	if(m_effect)
	{
		alDeleteEffects(1, &m_effect);
		m_effect = 0;
	}
	if(m_slot)
	{
		 alDeleteAuxiliaryEffectSlots(1, &m_slot);
		 m_slot = 0;
	}
}

void EchoZone::ChangeWidth(double width)
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
	m_renderObject->setColor( osg::Vec4(0.4f, 0.3f, 0.0f, 0.3f) );
	
	//add new drawable to geode
	m_geode->addDrawable( m_renderObject.get() );
	
}

double EchoZone::GetWidth(){return m_width;}
