#ifndef SOUND_PRODUCER_TRACK_H
#define SOUND_PRODUCER_TRACK_H


#include <wx/wx.h>
#include "soundproducer.h"
#include "double-track.h"

#include "stereo-audio-track.h"

#include <memory>
#include <wx/combobox.h>

#include <wx/arrstr.h>

#include "soundproducer-registry.h"

//class to manipulate x,y z position of sound producer
class SoundProducerTrack : public Track
{
public:
	SoundProducerTrack(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext);
	
	StereoAudioTrack* GetReferenceToStereoAudioTrack();
	DoubleTrack* GetReferenceToXTrack();
	DoubleTrack* GetReferenceToYTrack();
	DoubleTrack* GetReferenceToZTrack();
	
	//functions to set reference to audio device and audio context to use for player
	void SetReferenceToAudioDevice(ALCdevice* thisAudioDevice);
	void SetReferenceToAudioContext(ALCcontext* thisAudioContext);
	
	//SoundProducer Editing
	void SetReferenceToSoundProducerToManipulate(SoundProducer* thisSoundProducer);
	void SetReferenceToSoundProducerRegistry(SoundProducerRegistry* thisRegistry);
	
	void UpdateComboBoxListFromSoundProducerRegistry();
	
	wxComboBox* GetReferenceToComboBox();
	
	//audio track related functions
	void SetupAxisForAudio(double& start, double& end,double& resolution, int& numTick);
	void SetReferenceToPlaybackControls(PlaybackControls* controls);
	
	//Double Track related functions
	void SetupAxisForVariable(double& start, double& end, double& resolution, int& numTick);
	
	void OnLeftMouseClick(wxMouseEvent& event);
	void OnRightMouseClick(wxCommandEvent& event);
	
	//Functions inherited from Track
	virtual void InitTrack(wxWindow* parent, std::vector <int> *timeTickVector);

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnScroll(wxScrollEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	
	void SetReferenceToCurrentTimeVariable(double* thisTimeVariable);
	void SetReferenceToTimeTickVector(std::vector <int> *thisVector);
	
	std::vector <int> *GetReferenceToTimeTickVector();
	
	double GetCurrentTime();
	
	//function to call in timer loop, variable to manipulate gets changed here
	virtual void FunctionToCallInPlayState();
    virtual void FunctionToCallInPauseState();
    virtual void FunctionToCallInRewindState();
    virtual void FunctionToCallInFastForwardState();
    virtual void FunctionToCallInNullState();
	
private:
	SoundProducer* soundProducerToManipulatePtr;
	
	OpenALSoftPlayer* audioPlayer;
	
	ALCdevice* audioDevicePtr; //pointer to audio device to be used
    ALCcontext* alContextPtr; //pointer to context of where audio is played
	
	StereoAudioTrack* audioTrack;
	DoubleTrack* xTrack;
	DoubleTrack* yTrack;
	DoubleTrack* zTrack;
	
	//variables to hold temporary values for sound producer position
	double tempX,tempY,tempZ;
	
	//source to play buffer
	ALuint track_source;
	
	wxComboBox* m_combo_box;
	
	SoundProducerRegistry* soundproducer_registry_ptr;
	
	void OnSelectedSoundProducerInComboBox(wxCommandEvent& event);
	
};

#endif
