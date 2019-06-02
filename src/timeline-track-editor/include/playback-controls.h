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
	int GetCurrentState();
	void SetReferenceToTimelineWindow(TimelineWindow* thisTimeline);
	
	enum 
	{
		STATE_NULL = 0,
		STATE_PLAY,
		STATE_PAUSE,
		STATE_REWIND,
		STATE_FAST_FORWARD,
		ID_PLAY_BUTTON_HIT,
		ID_PAUSE_BUTTON_HIT,
		ID_STOP_BUTTON_HIT,
		ID_FAST_FORWARD_BUTON_HIT,
		ID_REWIND_BUTTON_HIT
	};
	
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

	void AddFunctionToTimerLoop( std::function < void() > thisFunction);
	
private:
	PlaybackControls* m_controls;
	//vector of functions to call everytim Notify is called
	std::vector < std::function < void() > > functionsNotify;
	
};


#endif
