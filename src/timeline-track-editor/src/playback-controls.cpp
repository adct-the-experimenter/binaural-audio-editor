#include "playback-controls.h"


PlaybackControls::PlaybackControls(wxWindow* parent) : wxWindow(parent, wxID_ANY)
{
	time_res_seconds = double(TIME_RESOLUTION) / 1000;
	time_rewind_seconds = time_res_seconds * double(REWIND_SPEED);
	time_fast_forward_seconds = time_res_seconds * double(FAST_FORWARD_SPEED);
	
	//make horizontal box to put buttons in
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	
	//initialize buttons
	m_play_button = new wxButton(this, wxID_NEW, wxT("Play"));;
    m_pause_button = new wxButton(this, wxID_NEW, wxT("Pause"));;
    m_stop_button = new wxButton(this, wxID_NEW, wxT("Stop"));;
    m_rewind_button = new wxButton(this, wxID_NEW, wxT("Rewind"));;
    m_fast_forward_button = new wxButton(this, wxID_NEW, wxT("Fast Forward"));;
    
    m_play_button->Bind(wxEVT_BUTTON,&PlaybackControls::Play,this);
    m_pause_button->Bind(wxEVT_BUTTON,&PlaybackControls::Pause,this);
    m_stop_button->Bind(wxEVT_BUTTON,&PlaybackControls::Stop,this);
    m_rewind_button->Bind(wxEVT_BUTTON,&PlaybackControls::Rewind,this);
    m_fast_forward_button->Bind(wxEVT_BUTTON,&PlaybackControls::FastForward,this);
    
    hbox->Add(m_play_button, 0,  wxALL, 0);
    hbox->Add(m_pause_button, 0,  wxALL, 0);
    hbox->Add(m_stop_button, 0,  wxALL, 0);
    hbox->Add(m_rewind_button, 0,  wxALL, 0);
    hbox->Add(m_fast_forward_button, 0, wxALL, 0);
    
    SetSizerAndFit(hbox);
    
	timelineWindowPtr = nullptr;
}

void PlaybackControls::RunPlaybackState()
{
	switch (current_state)
	{
		case STATE_NULL:{  break;}
		case STATE_PLAY:
		{
			PlaybackControls::PlayOP();
			break;
		}
		case STATE_PAUSE:
		{
			PlaybackControls::PauseOP();
			
			break;
		}
		case STATE_REWIND:
		{
			PlaybackControls::RewindOP();
			break;
			
		}
		case STATE_FAST_FORWARD:
		{
			PlaybackControls::FastForwardOP(); 
			break;
		}
	};
}



void PlaybackControls::Play(wxCommandEvent& event)
{
	current_state = STATE_PLAY;
}

void PlaybackControls::Pause(wxCommandEvent& event)
{
	current_state = STATE_PAUSE;
}

void PlaybackControls::Stop(wxCommandEvent& event)
{
	PlaybackControls::StopOP();
}

void PlaybackControls::Rewind(wxCommandEvent& event)
{
	current_state = STATE_REWIND;
}

void PlaybackControls::FastForward(wxCommandEvent& event)
{
	current_state = STATE_FAST_FORWARD;
}

void PlaybackControls::SetReferenceToTimelineWindow(TimelineWindow* thisTimeline){timelineWindowPtr = thisTimeline;}

void PlaybackControls::SetCurrentState(int state){current_state = state;}
int PlaybackControls::GetCurrentState(){return current_state;}

void PlaybackControls::SetCurrentTimePosition(double& thisTime)
{
	if(timelineWindowPtr != nullptr)
	{
		timelineWindowPtr->SetCurrentTimePosition(thisTime);
	}
}

void PlaybackControls::PlayOP()
{
	if(timelineWindowPtr != nullptr)
	{
		//if current time is past the end 
		if(timelineWindowPtr->GetCurrentTimePosition() <= TIME_END_VALUE)
		{
			//increment current time position until end, then set state to null
			double newTime = timelineWindowPtr->GetCurrentTimePosition() + time_res_seconds;
			timelineWindowPtr->SetCurrentTimePosition(newTime);
		}
		else
		{
			//set current time to end and put it in null state
			double newTime = TIME_END_VALUE;
			timelineWindowPtr->SetCurrentTimePosition(newTime);
			current_state = STATE_NULL;
		}
	} 
}

void PlaybackControls::PauseOP()
{
	if(timelineWindowPtr != nullptr)
	{	
		//do nothing
	}
}

void PlaybackControls::StopOP()
{
	//reset time back to start
	double newTime = 0.0;
	timelineWindowPtr->SetCurrentTimePosition(newTime);
	
	current_state = STATE_NULL;
}

void PlaybackControls::RewindOP()
{
	if(timelineWindowPtr != nullptr)
	{
		if(timelineWindowPtr->GetCurrentTimePosition() >= TIME_START_VALUE)
		{
			//decrement current time position until beginning, then set state to null
			double newTime = timelineWindowPtr->GetCurrentTimePosition() - time_rewind_seconds;
			timelineWindowPtr->SetCurrentTimePosition(newTime);
		}
		else
		{
			double newTime = TIME_START_VALUE;
			timelineWindowPtr->SetCurrentTimePosition(newTime);
			current_state = STATE_NULL;
		}
	}
	
}

void PlaybackControls::FastForwardOP()
{
	if(timelineWindowPtr != nullptr)
	{
		//if current time is past the end 
		if(timelineWindowPtr->GetCurrentTimePosition() <= TIME_END_VALUE)
		{
			//increment current time position until end, then set state to null
			double newTime = timelineWindowPtr->GetCurrentTimePosition() + time_fast_forward_seconds;
			timelineWindowPtr->SetCurrentTimePosition(newTime);
		}
		else
		{
			//set current time to end and put it in null state
			double newTime = TIME_END_VALUE;
			timelineWindowPtr->SetCurrentTimePosition(newTime);
			current_state = STATE_NULL;
		}
	}
}

//Playback Timer

PlaybackTimer::PlaybackTimer(PlaybackControls* controls) : wxTimer()
{
    m_controls = controls;
}

void PlaybackTimer::Notify()
{
    m_controls->RunPlaybackState(); 
    
    switch(m_controls->GetCurrentState())
    {
		case PlaybackControls::STATE_PLAY:
		{
			for (auto& x: functionsPlayState) {x();} 
			break;
		}
		
		case PlaybackControls::STATE_PAUSE:
		{
			for (auto& x: functionsPauseState) {x();} 
			break;
		}
		
		case PlaybackControls::STATE_REWIND:
		{
			for (auto& x: functionsRewindState) {x();} 
			break;
		}
		
		case PlaybackControls::STATE_FAST_FORWARD:
		{
			for (auto& x: functionsFastForwardState) {x();} 
			break;
		}
		
		case PlaybackControls::STATE_NULL:
		{
			for (auto& x: functionsNullState) {x();} 
			break;
		}
	}
    
}

void PlaybackTimer::start()
{
    wxTimer::Start(TIME_RESOLUTION,wxTIMER_CONTINUOUS); //the timer calls Notify every TIMER_INTERVAL milliseconds
}

void PlaybackTimer::AddFunctionToTimerLoopPlayState( std::function < void() > thisFunction)
{
	functionsPlayState.push_back(thisFunction);
}

void PlaybackTimer::AddFunctionToTimerLoopPauseState( std::function < void() > thisFunction)
{
	functionsPauseState.push_back(thisFunction);
}

void PlaybackTimer::AddFunctionToTimerLoopRewindState( std::function < void() > thisFunction)
{
	functionsRewindState.push_back(thisFunction);
}

void PlaybackTimer::AddFunctionToTimerLoopFastForwardState( std::function < void() > thisFunction)
{
	functionsFastForwardState.push_back(thisFunction);
}

void PlaybackTimer::AddFunctionToTimerLoopNullState( std::function < void() > thisFunction)
{
	functionsNullState.push_back(thisFunction);
}

