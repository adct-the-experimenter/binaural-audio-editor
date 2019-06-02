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
    
    //Track related functions
    void AddTrack(Track* thisTrack, int& space);
    
    void AddTrackFunctionToCallInTimerLoop(Track* thisTrack);
    
    //Display related functions
    void AddSpacerBlock(int space); //function to add space between boxes containing Tracks or panels
    void AddText(wxString thisText, wxPoint thisPoint);//function to add text anywhere in the window
    
    void OnClose(wxCloseEvent& evt);
    
private:
	TimelineWindow* timelineWindowPtr;
	PlaybackTimer* timer;
	PlaybackControls* controls;
	
	DECLARE_EVENT_TABLE()
	
};

#endif
