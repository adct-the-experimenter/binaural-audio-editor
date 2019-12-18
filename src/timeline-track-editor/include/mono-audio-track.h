#ifndef MONOAUDIOTRACK_H
#define MONOAUDIOTRACK_H

#include "track.h"
#include "audio-track.h"

class MonoAudioTrack : public Track
{
public:
	MonoAudioTrack(const wxString& title);
	
	void SetReferenceToSourceToManipulate(ALuint* source);
	void SetReferenceToAudioPlayer(OpenALSoftPlayer* thisPlayer);
	
	void SetReferenceToBrowseButton(wxButton* thisButton);
	
	enum State
    {
		PLAYER_NULL = 0,
		PLAYER_STOPPED,
		PLAYER_PLAYING,
		PLAYER_PAUSED,
		PLAYER_REWINDING,
		PLAYER_FAST_FORWARDING
	};
	
	void SetAudioTrackState(int thisState);
    int GetAudioTrackState();
	
	AudioTrack* GetReferenceToChannelTrack();
	
	enum Options
	{
		ONLY_BUFFER_AUDIO = 0, //for use with multiple stereo audio track in use and only need to buffer multiple sources, play sources all in sync with PlayMultipleUpdatedBuffers
		BUFFER_AND_PLAY_AUDIO //for use with only one stereo audio track and can play single buffer
	};
	
	void SetTrackOption(int thisOption);
	int GetTrackOption();
	
	//Audio Track related functions
	
	
	//Track related functions
	virtual void InitTrack(wxWindow* parent, std::vector <int> *timeTickVector);
	
	//function to set bounds for variable to change as well as number of ticks to draw
	virtual void SetupAxisForVariable(double& start, double& end, double& resolution, int& numTick);
	
	
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnScroll(wxScrollEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	void OnLeftMouseClick(wxMouseEvent& event);
    void OnRightMouseClick(wxCommandEvent& event);
    
    void SetReferenceToCurrentTimeVariable(double* thisTimeVariable);
    void SetReferenceToTimeTickVector(std::vector <int> *thisVector);
    
    void SetReferenceToPlaybackControls(PlaybackControls* controls);
	PlaybackControls* GetReferenceToPlaybackControls();
    
	void SetTitle(wxString thisTitle);
    wxString GetTitle();
    
    std::vector <int> *GetReferenceToTimeTickVector();
    
    double GetCurrentTime();
    
    //function to call in timer loop, variable to manipulate gets changed here
    virtual void FunctionToCallInPlayState();
    virtual void FunctionToCallInPauseState();
    virtual void FunctionToCallInRewindState();
    virtual void FunctionToCallInFastForwardState();
    virtual void FunctionToCallInNullState();
    
    void SetFunctionToCallAfterVariableChange(std::function < void() > thisFunction); 
    
    void render(wxDC& dc);
    void logic_left_click();
    void logic_right_click();
	
private:

	AudioTrack* m_channel_track;
	
	//state of audio track
	int track_state;
	
	ALuint* sourceToManipulatePtr;
	OpenALSoftPlayer* audioPlayerPtr;
	
	//options for track
	int track_options;
	
	double verticalStart; //vertical axis start
	double verticalEnd; //vertical axis end
	double verticalResolution;
	int verticalNumTicks; //number of tick marks in vertical axis
	
	std::function < void() > func_after_var_change;
	
	wxButton* browseButton;
    void OnBrowse(wxCommandEvent& event);
    
    std::string inputSoundFilePath;
    std::string streamSoundFilePath;
    
    PlaybackControls* playbackControlsPtr;
    
    void al_nssleep(unsigned long nsec);
    
};

#endif
