#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include "track.h"
#include "audio-graph.h"

#include <sndfile.h>

#include "openalsoft-player.h"

#include <functional>   // std::function, std::negate

#include "audio-stream-container.h"

#include "playback-controls.h"

#define	BUFFER_LEN	1024

#define	MAX_CHANNELS	2

//Class used to manipulate placement of audio samples in timeline.
//Used as helper class for StereoAudioTrack and MonoAudioTrack

class AudioTrack : public Track
{

public:
    AudioTrack(const wxString& title);
    
//Audio related functions
	void SetReferenceToSourceToManipulate(ALuint* thisSource);
	void SetReferenceToAudioPlayer(OpenALSoftPlayer* thisPlayer);
	
	int GetNumberOfChannelsInAudioFile(std::string path,SF_INFO& input_sfinfo);
    
    
    void ReadAndCopyDataFromInputFile(std::vector<double> *audio_data_input_copy_ptr,std::string inputSoundFilePath,SF_INFO& input_sfinfo);
    
    void CopyInputDataIntoAudioDataStream(std::vector<double> *audio_data_input_copy_ptr, AudioStreamContainer* audio_data_stream_ptr,std::string streamSoundFilePath,SF_INFO& input_sfinfo);
    
    void PlotOneChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data_stream_ptr,SF_INFO& input_sfinfo);
    void PlotLeftChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data_stream_ptr,SF_INFO& input_sfinfo);
    void PlotRightChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data_stream_ptr,SF_INFO& input_sfinfo);
    
//Track related Functions
    virtual void InitTrack(wxWindow* parent, std::vector <int> *timeTickVector);
	
	//function to set bounds for variable to change as well as number of ticks to draw
	virtual void SetupAxisForVariable(double& start, double& end, double& resolution, int& numTick);
	
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnScroll(wxScrollEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	
    virtual void OnLeftMouseClick(wxMouseEvent& event);
    virtual void OnRightMouseClick(wxCommandEvent& event);
    
    void SetReferenceToCurrentTimeVariable(double* thisTimeVariable);
    
    void SetReferenceToTimeTickVector(std::vector <int> *thisVector);
    std::vector <int> *GetReferenceToTimeTickVector();
    
    void SetReferenceToPlaybackControls(PlaybackControls* controls);
	PlaybackControls* GetReferenceToPlaybackControls();
    
    void SetTitle(wxString thisTitle);
    wxString GetTitle();
    
    double GetCurrentTime();
    
    virtual void FunctionToCallInPlayState();
    virtual void FunctionToCallInPauseState();
    virtual void FunctionToCallInRewindState();
    virtual void FunctionToCallInFastForwardState();
    virtual void FunctionToCallInNullState();
    
    void SetFunctionToCallAfterVariableChange(std::function < void() > thisFunction); 
    
    void render(wxDC& dc);
    void logic_left_click();
    void logic_right_click();
    
    enum State
    {
		PLAYER_NULL = 0,
		PLAYER_PLAYING,
		PLAYER_PAUSED,
		PLAYER_REWINDING
	};
	
	void SetAudioTrackState(int thisState);
    int GetAudioTrackState();
    
    void ClearGraph();
    
private:
	
	//Audio Processes and Operations
	
	//state of audio track
	int track_state;

	//source to manipulate
	ALuint* sourceToManipulatePtr;
	
	//pointer to audio player to use
	OpenALSoftPlayer* audioPlayerPtr;
	
	
	//File handlers for input file and file to stream
	SNDFILE *inputFile;
	
	
	//GUI
	
	std::vector <double> m_vertical_var_num;
	void InitVerticalAxis();
	std::vector<double> LinearSpacedArray(double a, double b, std::size_t N);
	
	double verticalStart; //vertical axis start
	double verticalEnd; //vertical axis end
	double verticalResolution;
	int verticalNumTicks; //number of tick marks in vertical axis
    
    AudioGraph* m_audio_graph;
    
    std::function < void() > func_after_var_change;
    
    void al_nssleep(unsigned long nsec);
    
    PlaybackControls* playbackControlsPtr;
};

#endif
