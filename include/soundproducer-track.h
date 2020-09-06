#ifndef SOUND_PRODUCER_TRACK_H
#define SOUND_PRODUCER_TRACK_H


#include "double-track.h"

#include "stereo-audio-track.h"

#include <memory>
#include <wx/combobox.h>

#include <wx/arrstr.h>

#include "soundproducer-registry.h"
#include "soundproducer.h"

struct SoundProducerTrackSaveData
{
	//name of sound producer
	std::string soundproducer_name;
	
	//double track save data
	DDMap* time_value_map_x_ptr;
	DDMap* time_value_map_y_ptr;
	DDMap* time_value_map_z_ptr;
	
	//file name of sound file
	std::string soundfilepath;
};

//class to manipulate x,y z position of sound producer
class SoundProducerTrack : public Track
{
public:
	SoundProducerTrack(const wxString& title,ALCdevice* thisAudioDevice,ALCcontext* thisAudioContext);
	~SoundProducerTrack();

	StereoAudioTrack* GetReferenceToStereoAudioTrack();
	DoubleTrack* GetReferenceToXTrack();
	DoubleTrack* GetReferenceToYTrack();
	DoubleTrack* GetReferenceToZTrack();
	
	void BufferAndPlayAudio(double& current_time);
	void StopAudio();

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
	
	//functions to return reference to track source and sound producer manipulated by soundproducer track
	//for use with effects manager
    ALuint* GetReferenceToTrackSource();
	SoundProducer* GetReferenceToSoundProducerManipulated();
	bool IsReverbApplied();
	void SetStatusReverbApplied(bool status);
	
	//Audio DAW 
	void SetReferenceToImportAudioDAWButton(wxButton* thisButton);
	
	SoundProducerTrackSaveData GetSoundProducerTrackSaveData();
	void LoadSoundProducerTrackSaveData(SoundProducerTrackSaveData& data);
	
	void SelectSoundProducerByName(std::string name);
	
	void SetComboBoxToThisSelectionName(std::string name);

	friend class SoundProducerTrackManager;
	
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

	
	//bool to tell if reverb is applied to the source of the track
	bool reverbApplied;
	
	//button for using the DAW audio import plugin for binaural audio editor
	wxButton* m_importAudioDAWButton;
	
	void OnImportAudioDAWButtonClick(wxCommandEvent& event);
	
	std::string streamSoundFilePath;
	
	SoundProducerTrackSaveData m_saveData;
};

#endif
