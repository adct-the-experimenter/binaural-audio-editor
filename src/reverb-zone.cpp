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


ReverbZone::ReverbZone() : EffectZone()
{
	m_effect = 0;
	m_slot = 0;
	
	m_type = ReverbZone::Type::NONE;
	
	standardColor.r = 0.9f;
	standardColor.g = 0.8f;
	standardColor.b = 0.0f;
	standardColor.alpha = 0.3f;
	
	eaxColor.r = 0.6f;
	eaxColor.g = 0.0f;
	eaxColor.b = 0.1f;
	eaxColor.alpha = 0.3f;
	
}

ReverbZone::~ReverbZone()
{
	if(m_effect != 0 && m_slot != 0)
	{	
		FreeEffects();
	}
}

/* LoadEffect loads the given reverb properties into a new OpenAL effect
 * object, and returns the new effect ID. */
ALuint ReverbZone::LoadStandardReverbEffect(const EFXEAXREVERBPROPERTIES *reverb)
{
	#define LOAD_PROC(T, x)  ((x) = (T)alGetProcAddress(#x))
		LOAD_PROC(LPALGENEFFECTS, alGenEffects);
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
	
    ALuint effect = 0;
    ALenum err;

    /* Create the effect object and check if we can do EAX reverb. */
    alGenEffects(1, &effect);
    
    
	printf("Using Standard Reverb\n");

	/* No EAX Reverb. Set the standard reverb effect type then load the
	 * available reverb properties. */
	alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

	alEffectf(effect, AL_REVERB_DENSITY, reverb->flDensity);
	alEffectf(effect, AL_REVERB_DIFFUSION, reverb->flDiffusion);
	alEffectf(effect, AL_REVERB_GAIN, reverb->flGain);
	alEffectf(effect, AL_REVERB_GAINHF, reverb->flGainHF);
	alEffectf(effect, AL_REVERB_DECAY_TIME, reverb->flDecayTime);
	alEffectf(effect, AL_REVERB_DECAY_HFRATIO, reverb->flDecayHFRatio);
	alEffectf(effect, AL_REVERB_REFLECTIONS_GAIN, reverb->flReflectionsGain);
	alEffectf(effect, AL_REVERB_REFLECTIONS_DELAY, reverb->flReflectionsDelay);
	alEffectf(effect, AL_REVERB_LATE_REVERB_GAIN, reverb->flLateReverbGain);
	alEffectf(effect, AL_REVERB_LATE_REVERB_DELAY, reverb->flLateReverbDelay);
	alEffectf(effect, AL_REVERB_AIR_ABSORPTION_GAINHF, reverb->flAirAbsorptionGainHF);
	alEffectf(effect, AL_REVERB_ROOM_ROLLOFF_FACTOR, reverb->flRoomRolloffFactor);
	alEffecti(effect, AL_REVERB_DECAY_HFLIMIT, reverb->iDecayHFLimit);
    

    /* Check if an error occured, and clean up if so. */
    err = alGetError();
    if(err != AL_NO_ERROR)
    {
        fprintf(stderr, "OpenAL error: %s\n", alGetString(err));
        if(alIsEffect(effect))
            alDeleteEffects(1, &effect);
        return 0;
    }

    return effect;
}

ALuint ReverbZone::LoadEAXReverbEffect(const EFXEAXREVERBPROPERTIES *reverb)
{
	#define LOAD_PROC(T, x)  ((x) = (T)alGetProcAddress(#x))
		LOAD_PROC(LPALGENEFFECTS, alGenEffects);
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
	
	ALuint effect = 0;
    ALenum err;
    
	/* Create the effect object and check if we can do EAX reverb. */
    alGenEffects(1, &effect);
    
	if(alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)
    {
        printf("Using EAX Reverb\n");

        /* EAX Reverb is available. Set the EAX effect type then load the
         * reverb properties. */
        alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);

        alEffectf(effect, AL_EAXREVERB_DENSITY, reverb->flDensity);
        alEffectf(effect, AL_EAXREVERB_DIFFUSION, reverb->flDiffusion);
        alEffectf(effect, AL_EAXREVERB_GAIN, reverb->flGain);
        alEffectf(effect, AL_EAXREVERB_GAINHF, reverb->flGainHF);
        alEffectf(effect, AL_EAXREVERB_GAINLF, reverb->flGainLF);
        alEffectf(effect, AL_EAXREVERB_DECAY_TIME, reverb->flDecayTime);
        alEffectf(effect, AL_EAXREVERB_DECAY_HFRATIO, reverb->flDecayHFRatio);
        alEffectf(effect, AL_EAXREVERB_DECAY_LFRATIO, reverb->flDecayLFRatio);
        alEffectf(effect, AL_EAXREVERB_REFLECTIONS_GAIN, reverb->flReflectionsGain);
        alEffectf(effect, AL_EAXREVERB_REFLECTIONS_DELAY, reverb->flReflectionsDelay);
        alEffectfv(effect, AL_EAXREVERB_REFLECTIONS_PAN, reverb->flReflectionsPan);
        alEffectf(effect, AL_EAXREVERB_LATE_REVERB_GAIN, reverb->flLateReverbGain);
        alEffectf(effect, AL_EAXREVERB_LATE_REVERB_DELAY, reverb->flLateReverbDelay);
        alEffectfv(effect, AL_EAXREVERB_LATE_REVERB_PAN, reverb->flLateReverbPan);
        alEffectf(effect, AL_EAXREVERB_ECHO_TIME, reverb->flEchoTime);
        alEffectf(effect, AL_EAXREVERB_ECHO_DEPTH, reverb->flEchoDepth);
        alEffectf(effect, AL_EAXREVERB_MODULATION_TIME, reverb->flModulationTime);
        alEffectf(effect, AL_EAXREVERB_MODULATION_DEPTH, reverb->flModulationDepth);
        alEffectf(effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, reverb->flAirAbsorptionGainHF);
        alEffectf(effect, AL_EAXREVERB_HFREFERENCE, reverb->flHFReference);
        alEffectf(effect, AL_EAXREVERB_LFREFERENCE, reverb->flLFReference);
        alEffectf(effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, reverb->flRoomRolloffFactor);
        alEffecti(effect, AL_EAXREVERB_DECAY_HFLIMIT, reverb->iDecayHFLimit);
    }
    
     /* Check if an error occured, and clean up if so. */
    err = alGetError();
    if(err != AL_NO_ERROR)
    {
        fprintf(stderr, "OpenAL error: %s\n", alGetString(err));
        if(alIsEffect(effect))
            alDeleteEffects(1, &effect);
        return 0;
    }
    
    return effect;
}

void ReverbZone::InitStandardReverbZone(std::string& thisName,
									double& x, double& y, double& z, double& width,
									ReverbStandardProperties& properties)
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
	
	//initialize reverb property
	EFXEAXREVERBPROPERTIES reverb = EFX_REVERB_PRESET_GENERIC;
	
	//assign custom properties
	reverb.flDensity = properties.flDensity;
	reverb.flDiffusion = properties.flDiffusion;
	reverb.flGain = properties.flGain;
	reverb.flGainHF = properties.flGainHF;
	reverb.flDecayTime = properties.flDecayTime;
	reverb.flDecayHFRatio = properties.flDecayHFRatio;
	reverb.flReflectionsGain = properties.flReflectionsGain;
	reverb.flReflectionsDelay = properties.flReflectionsDelay;
	reverb.flLateReverbGain = properties.flLateReverbGain;
	reverb.flLateReverbDelay = properties.flLateReverbDelay;
	reverb.flAirAbsorptionGainHF = properties.flAirAbsorptionGainHF;
	reverb.flRoomRolloffFactor = properties.flRoomRolloffFactor;
	
	//load effect based on type
	m_effect = LoadStandardReverbEffect(&reverb);
	
	/* Create the effect slot object. This is what "plays" an effect on sources
     * that connect to it. */
    alGenAuxiliaryEffectSlots(1, &m_slot);
    EffectZone::SetEffectsSlotPointer(&m_slot);

    /* Tell the effect slot to use the loaded effect object. Note that the this
     * effectively copies the effect properties. You can modify or delete the
     * effect object afterward without affecting the effect slot.
     */
    ALint i_effect = (ALint)(m_effect);
    alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, i_effect);
    assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
    
	EffectZone::InitEffectZone(thisName,x,y,z,width);
	
	//initialize type to Standard
	m_type = ReverbZone::Type::STANDARD;
	
	//initialize standard properties
	m_standard_prop = properties;
	
	
	//save data
	m_saveDataStandard.name = thisName;
	m_saveDataStandard.x = x;
	m_saveDataStandard.y = y;
	m_saveDataStandard.z = z;
	m_saveDataStandard.width = width;
	m_saveDataStandard.properties = properties;
}

void ReverbZone::InitStandardReverbZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width,
							ReverbStandardProperties& properties)
{
	
	ReverbZone::InitStandardReverbZone( thisName,
									x, y, z, width,
									properties);
	
	EffectZone::SetColor(standardColor);
	EffectZone::InitEffectZoneWithGraphicalObject(thisName,x,y,z,width,standardColor);
}

void ReverbZone::InitEAXReverbZone(std::string& thisName,
									double& x, double& y, double& z, double& width,
									ReverbEAXProperties& properties)
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
	
	//initialize reverb property
	EFXEAXREVERBPROPERTIES reverb = EFX_REVERB_PRESET_GENERIC;
	
	//assign custom properties
	reverb.flDensity = properties.flDensity;
	reverb.flDiffusion = properties.flDiffusion;
	reverb.flGain = properties.flGain;
	reverb.flGainHF = properties.flGainHF;
	reverb.flDecayTime = properties.flDecayTime;
	reverb.flDecayHFRatio = properties.flDecayHFRatio;
	reverb.flReflectionsGain = properties.flReflectionsGain;
	reverb.flReflectionsDelay = properties.flReflectionsDelay;
	reverb.flLateReverbGain = properties.flLateReverbGain;
	reverb.flLateReverbDelay = properties.flLateReverbDelay;
	
	reverb.flGainLF = properties.flGainLF;
	reverb.flDecayLFRatio = properties.flDecayLFRatio;
	reverb.flEchoDepth = properties.flEchoDepth;
	reverb.flEchoTime = properties.flEchoTime;
	reverb.flModulationDepth = properties.flModulationDepth;
	reverb.flModulationTime = properties.flModulationTime;
	reverb.flLFReference = properties.flLFReference;
	reverb.flHFReference = properties.flHFReference;
	
	reverb.flAirAbsorptionGainHF = properties.flAirAbsorptionGainHF;
	reverb.flRoomRolloffFactor = properties.flRoomRolloffFactor;
	
	//load effect based on type
	m_effect = LoadEAXReverbEffect(&reverb);
	
	
	/* Create the effect slot object. This is what "plays" an effect on sources
     * that connect to it. */
    alGenAuxiliaryEffectSlots(1, &m_slot );
    
    EffectZone::SetEffectsSlotPointer(&m_slot);

    /* Tell the effect slot to use the loaded effect object. Note that the this
     * effectively copies the effect properties. You can modify or delete the
     * effect object afterward without affecting the effect slot.
     */
    ALint i_effect = (ALint)(m_effect);
    alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT,i_effect );
    assert(alGetError()==AL_NO_ERROR && "Failed to set effect slot");
    
	EffectZone::InitEffectZone(thisName,x,y,z,width);
	
	//initialize type to EAX
	m_type = ReverbZone::Type::EAX;
	
	//initialize eax properties
	m_eax_prop = properties;
	
	//save data
	m_saveDataEAX.name = thisName;
	m_saveDataEAX.x = x;
	m_saveDataEAX.y = y;
	m_saveDataEAX.z = z;
	m_saveDataEAX.width = width;
	m_saveDataEAX.properties = properties;
	
}

void ReverbZone::InitEAXReverbZoneWithGraphicalObject(std::string& thisName,
							double& x, double& y, double& z, double& width,
							ReverbEAXProperties& properties)
{
	
	ReverbZone::InitEAXReverbZone(thisName,
									x, y, z, width,
									properties);
	
	EffectZone::SetColor(eaxColor);
	EffectZone::InitEffectZoneWithGraphicalObject(thisName,x,y,z,width,eaxColor);
}

void ReverbZone::ChangeStandardReverbZoneProperties(ReverbStandardProperties& properties)
{
	
	//initialize reverb property
	EFXEAXREVERBPROPERTIES reverb = EFX_REVERB_PRESET_GENERIC;
	
	//assign custom properties
	reverb.flDensity = properties.flDensity;
	reverb.flDiffusion = properties.flDiffusion;
	reverb.flGain = properties.flGain;
	reverb.flGainHF = properties.flGainHF;
	reverb.flDecayTime = properties.flDecayTime;
	reverb.flDecayHFRatio = properties.flDecayHFRatio;
	reverb.flReflectionsGain = properties.flReflectionsGain;
	reverb.flReflectionsDelay = properties.flReflectionsDelay;
	reverb.flLateReverbGain = properties.flLateReverbGain;
	reverb.flLateReverbDelay = properties.flLateReverbDelay;
	reverb.flAirAbsorptionGainHF = properties.flAirAbsorptionGainHF;
	reverb.flRoomRolloffFactor = properties.flRoomRolloffFactor;
	
	printf("Using Standard Reverb\n");

	/* No EAX Reverb. Set the standard reverb effect type then load the
	 * available reverb properties. */
	alEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB); 
	
	alEffectf(m_effect, AL_REVERB_DENSITY, reverb.flDensity);
	alEffectf(m_effect, AL_REVERB_DIFFUSION, reverb.flDiffusion);
	alEffectf(m_effect, AL_REVERB_GAIN, reverb.flGain);
	alEffectf(m_effect, AL_REVERB_GAINHF, reverb.flGainHF);
	alEffectf(m_effect, AL_REVERB_DECAY_TIME, reverb.flDecayTime);
	alEffectf(m_effect, AL_REVERB_DECAY_HFRATIO, reverb.flDecayHFRatio);
	alEffectf(m_effect, AL_REVERB_REFLECTIONS_GAIN, reverb.flReflectionsGain);
	alEffectf(m_effect, AL_REVERB_REFLECTIONS_DELAY, reverb.flReflectionsDelay);
	alEffectf(m_effect, AL_REVERB_LATE_REVERB_GAIN, reverb.flLateReverbGain);
	alEffectf(m_effect, AL_REVERB_LATE_REVERB_DELAY, reverb.flLateReverbDelay);
	alEffectf(m_effect, AL_REVERB_AIR_ABSORPTION_GAINHF, reverb.flAirAbsorptionGainHF);
	alEffectf(m_effect, AL_REVERB_ROOM_ROLLOFF_FACTOR, reverb.flRoomRolloffFactor);
	alEffecti(m_effect, AL_REVERB_DECAY_HFLIMIT, reverb.iDecayHFLimit);
	
    
    
    
    //load the newly modified effect into the effect slot
	
	/* Tell the effect slot to use the loaded effect object. Note that the this
     * effectively copies the effect properties. You can modify or delete the
     * effect object afterward without affecting the effect slot.
     */
    ALint i_effect = (ALint)(m_effect);
    alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, i_effect);
    
    m_standard_prop = properties;
    m_saveDataStandard.properties = properties;
    
}

void ReverbZone::ChangeEAXReverbZoneProperties(ReverbEAXProperties& properties)
{
	
	
	//initialize reverb property
	EFXEAXREVERBPROPERTIES reverb = EFX_REVERB_PRESET_GENERIC;
	
	//assign custom properties
	reverb.flDensity = properties.flDensity;
	reverb.flDiffusion = properties.flDiffusion;
	reverb.flGain = properties.flGain;
	reverb.flGainHF = properties.flGainHF;
	reverb.flDecayTime = properties.flDecayTime;
	reverb.flDecayHFRatio = properties.flDecayHFRatio;
	reverb.flReflectionsGain = properties.flReflectionsGain;
	reverb.flReflectionsDelay = properties.flReflectionsDelay;
	reverb.flLateReverbGain = properties.flLateReverbGain;
	reverb.flLateReverbDelay = properties.flLateReverbDelay;
	
	reverb.flGainLF = properties.flGainLF;
	reverb.flDecayLFRatio = properties.flDecayLFRatio;
	reverb.flEchoDepth = properties.flEchoDepth;
	reverb.flEchoTime = properties.flEchoTime;
	reverb.flModulationDepth = properties.flModulationDepth;
	reverb.flModulationTime = properties.flModulationTime;
	reverb.flLFReference = properties.flLFReference;
	reverb.flHFReference = properties.flHFReference;
	
	reverb.flAirAbsorptionGainHF = properties.flAirAbsorptionGainHF;
	reverb.flRoomRolloffFactor = properties.flRoomRolloffFactor;
	
	if(alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)
    {
        printf("Using EAX Reverb\n");

        /* EAX Reverb is available. Set the EAX effect type then load the
         * reverb properties. */
        //alEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB); assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");

        alEffectf(m_effect, AL_EAXREVERB_DENSITY, reverb.flDensity); assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
        alEffectf(m_effect, AL_EAXREVERB_DIFFUSION, reverb.flDiffusion); assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
        alEffectf(m_effect, AL_EAXREVERB_GAIN, reverb.flGain); assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
        alEffectf(m_effect, AL_EAXREVERB_GAINHF, reverb.flGainHF); assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
        alEffectf(m_effect, AL_EAXREVERB_GAINLF, reverb.flGainLF); assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
        alEffectf(m_effect, AL_EAXREVERB_DECAY_TIME, reverb.flDecayTime); assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
        alEffectf(m_effect, AL_EAXREVERB_DECAY_HFRATIO, reverb.flDecayHFRatio);
        alEffectf(m_effect, AL_EAXREVERB_DECAY_LFRATIO, reverb.flDecayLFRatio);
        alEffectf(m_effect, AL_EAXREVERB_REFLECTIONS_GAIN, reverb.flReflectionsGain);
        alEffectf(m_effect, AL_EAXREVERB_REFLECTIONS_DELAY, reverb.flReflectionsDelay);
        alEffectfv(m_effect, AL_EAXREVERB_REFLECTIONS_PAN, reverb.flReflectionsPan);
        alEffectf(m_effect, AL_EAXREVERB_LATE_REVERB_GAIN, reverb.flLateReverbGain);
        alEffectf(m_effect, AL_EAXREVERB_LATE_REVERB_DELAY, reverb.flLateReverbDelay);
        alEffectfv(m_effect, AL_EAXREVERB_LATE_REVERB_PAN, reverb.flLateReverbPan);
        alEffectf(m_effect, AL_EAXREVERB_ECHO_TIME, reverb.flEchoTime);
        alEffectf(m_effect, AL_EAXREVERB_ECHO_DEPTH, reverb.flEchoDepth);
        alEffectf(m_effect, AL_EAXREVERB_MODULATION_TIME, reverb.flModulationTime);
        alEffectf(m_effect, AL_EAXREVERB_MODULATION_DEPTH, reverb.flModulationDepth);
        alEffectf(m_effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, reverb.flAirAbsorptionGainHF);
        alEffectf(m_effect, AL_EAXREVERB_HFREFERENCE, reverb.flHFReference);
        alEffectf(m_effect, AL_EAXREVERB_LFREFERENCE, reverb.flLFReference);
        alEffectf(m_effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, reverb.flRoomRolloffFactor);
        alEffecti(m_effect, AL_EAXREVERB_DECAY_HFLIMIT, reverb.iDecayHFLimit);
        
    
		 //load the newly modified effect into the effect slot
		
		/* Tell the effect slot to use the loaded effect object. Note that the this
		 * effectively copies the effect properties. You can modify or delete the
		 * effect object afterward without affecting the effect slot.
		 */
		ALint i_effect = (ALint)(m_effect);
		alAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, i_effect);
		assert(alGetError()== AL_NO_ERROR && "Failed to set effect slot");
		
		m_eax_prop = properties;
		m_saveDataEAX.properties = properties;
    }
    
    
}


ReverbStandardProperties& ReverbZone::GetStandardReverbZoneProperties()
{
	return m_standard_prop;
}

ReverbEAXProperties& ReverbZone::GetEAXReverbZoneProperties()
{
	return m_eax_prop;
}


void ReverbZone::SetType(ReverbZone::Type& type){m_type = type;}
ReverbZone::Type& ReverbZone::GetType(){return m_type;}


ALuint* ReverbZone::GetEffectPointer(){ return &m_effect;}
ALuint* ReverbZone::GetEffectsSlotPointer(){std::cout << "Derived class called! \n"; return &m_slot;}

ALuint ReverbZone::GetEffect(){return m_effect;}
ALuint ReverbZone::GetEffectsSlot(){return m_slot;}

void ReverbZone::FreeEffects()
{
	if(m_effect != 0)
	{
		alDeleteEffects(1, &m_effect);
		m_effect = 0;
	}
	if(m_slot != 0)
	{
		 alDeleteAuxiliaryEffectSlots(1, &m_slot);
		 m_slot = 0;
	}
}


StandardReverbZoneSaveData ReverbZone::GetStandardReverbZoneSaveData()
{
	return m_saveDataStandard;
}

void ReverbZone::LoadStandardReverbZoneSaveData(StandardReverbZoneSaveData& data)
{
	m_saveDataStandard = data;
}

EAXReverbZoneSaveData ReverbZone::GetEAXReverbZoneSaveData()
{
	return m_saveDataEAX;
}

void ReverbZone::LoadEAXReverbZoneSaveData(EAXReverbZoneSaveData& data)
{
	m_saveDataEAX = data;
	
	
}

void ReverbZone::SetPositionX(double& x)
{
	m_saveDataStandard.x = x; 
	m_saveDataEAX.x = x; 
	EffectZone::SetPositionX(x);
}

void ReverbZone::SetPositionY(double& y)
{
	m_saveDataStandard.y = y; 
	m_saveDataEAX.y = y; 
	EffectZone::SetPositionY(y);
} 

void ReverbZone::SetPositionZ(double& z)
{
	m_saveDataStandard.z = z; 
	m_saveDataEAX.z = z; 
	EffectZone::SetPositionZ(z);
} 

void ReverbZone::ChangeWidth(double width)
{
	m_saveDataStandard.width = width; 
	m_saveDataEAX.width = width; 
	EffectZone::ChangeWidth(width);
}
