#ifndef TIMELINE_FRAME_H
#define TIMELINE_FRAME_H

#include "playback-controls.h"
#include "timeline-window.h"

#include <functional>   // std::function, std::negate

// Frame that contains Timeline scrolling window and playback controls
class TimelineFrame : public wxFrame
{
public:
    TimelineFrame(wxWindow *parent);
    ~TimelineFrame();
    
    TimelineWindow* GetTimelineWindow();
    
    PlaybackControls* GetPlaybackControlsReference();
    
    //Track related functions
    void AddTrack(Track* thisTrack, int& space);
    
    void AddTrackFunctionToCallInTimerLoopPlayState(Track* thisTrack);
    void AddTrackFunctionToCallInTimerLoopPauseState(Track* thisTrack);
    void AddTrackFunctionToCallInTimerLoopRewindState(Track* thisTrack);
    void AddTrackFunctionToCallInTimerLoopFastForwardState(Track* thisTrack);
    void AddTrackFunctionToCallInTimerLoopNullState(Track* thisTrack);
    
    //Display related functions
    void AddSpacerBlock(int space); //function to add space between boxes containing Tracks or panels
    void AddText(wxString thisText, wxPoint thisPoint);//function to add text anywhere in the window
    void AddBoxSizer(wxSizer *sizer, int proportion=0, int flag=0, int border=0, wxObject *userData=nullptr);
    
    void OnClose(wxCloseEvent& evt);
    
private:
	TimelineWindow* timelineWindowPtr;
	PlaybackTimer* timer;
	PlaybackControls* controls;
	
	DECLARE_EVENT_TABLE()
	
};

#endif
