#ifndef TIMELINE_FRAME_H
#define TIMELINE_FRAME_H

#include "playback-controls.h"
#include "timeline-window.h"

// Frame that contains Timeline scrolling window and playback controls
class TimelineFrame : public wxFrame
{
public:
    TimelineFrame(wxWindow *parent);
    ~TimelineFrame();
    
    TimelineWindow* GetTimelineWindow();
    
    void AddTrack(Track* thisTrack, int& space);
    
    void OnClose(wxCloseEvent& evt);
    
private:
	TimelineWindow* timelineWindowPtr;
	PlaybackTimer* timer;
	PlaybackControls* controls;
	
	DECLARE_EVENT_TABLE()
	
};

#endif
