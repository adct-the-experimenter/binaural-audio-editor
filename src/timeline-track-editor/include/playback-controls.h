#ifndef PLAYBACK_CONTROLS_H
#define PLAYBACK_CONTROLS_H

#include <wx/wx.h>
#include <wx/button.h>
#include <wx/timer.h>

#include <iostream>
#include <vector>

#include <functional>   // std::function, std::negate

#include "timeline-window.h"

class PlaybackControls : public wxWindow
{
public:
	PlaybackControls(wxWindow* parent);
	
	void RunPlaybackState();
	void SetCurrentState(int state);
	int GetCurrentState();
	void SetReferenceToTimelineWindow(TimelineWindow* thisTimeline);
	
	void SetCurrentTimePosition(double& thisTime);
	
	enum PlaybackState
	{
		STATE_NULL = 0,
		STATE_PLAY,
		STATE_PAUSE,
		STATE_REWIND,
		STATE_FAST_FORWARD
	};
	
	//operations to carry out in playback
	void PlayOP();
	void PauseOP();
	void StopOP();
	void RewindOP();
	void FastForwardOP();
	
private:
	
	TimelineWindow* timelineWindowPtr;
	
	int current_state;
	
	
	
	wxButton *m_play_button;
    wxButton *m_pause_button;
    wxButton *m_stop_button;
    wxButton *m_rewind_button;
    wxButton *m_fast_forward_button;
    
    void Play(wxCommandEvent& event);
	void Pause(wxCommandEvent& event);
	void Stop(wxCommandEvent& event);
	void Rewind(wxCommandEvent& event);
	void FastForward(wxCommandEvent& event);
	
	double time_res_seconds;
	double time_rewind_seconds;
	double time_fast_forward_seconds; 
};

//class to use with mainframe
class PlaybackTimer : public wxTimer
{
public:
    PlaybackTimer(PlaybackControls* controls);
    void Notify(); //action to take periodically after certain amount of time defined
    void start();

	void AddFunctionToTimerLoopPlayState( std::function < void() > thisFunction);
	void AddFunctionToTimerLoopPauseState( std::function < void() > thisFunction);
	void AddFunctionToTimerLoopRewindState( std::function < void() > thisFunction);
	void AddFunctionToTimerLoopFastForwardState( std::function < void() > thisFunction);
	void AddFunctionToTimerLoopNullState( std::function < void() > thisFunction);
	
private:
	PlaybackControls* m_controls;
	//vector of functions to call everytim Notify is called
	std::vector < std::function < void() > > functionsPlayState;
	std::vector < std::function < void() > > functionsPauseState;
	std::vector < std::function < void() > > functionsRewindState;
	std::vector < std::function < void() > > functionsFastForwardState;
	std::vector < std::function < void() > > functionsNullState;
	
	
};


#endif
