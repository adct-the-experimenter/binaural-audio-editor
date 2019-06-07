#include "playback-controls.h"


PlaybackControls::PlaybackControls(wxWindow* parent) : wxWindow(parent, wxID_ANY)
{
	time_res_seconds = double(TIME_RESOLUTION) / 1000;
	time_rewind_seconds = time_res_seconds * double(REWIND_SPEED);
	time_fast_forward_seconds = time_res_seconds * double(FAST_FORWARD_SPEED);
	
	//make horizontal box to put buttons in
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	
	//initialize buttons
	m_play_button = new wxButton(this, ID_PLAY_BUTTON_HIT, wxT("Play"));;
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
			break;
		}
		case STATE_PAUSE:
		{
			if(timelineWindowPtr != nullptr)
			{	
				//do nothing
				
			}
			
			break;
		}
		case STATE_REWIND:
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
			break;
			
		}
		case STATE_FAST_FORWARD:
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
	//reset time back to start
	double newTime = 0.0;
	timelineWindowPtr->SetCurrentTimePosition(newTime);
	
	current_state = STATE_NULL;
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

int PlaybackControls::GetCurrentState(){return current_state;}

PlaybackTimer::PlaybackTimer(PlaybackControls* controls) : wxTimer()
{
    m_controls = controls;
}

void PlaybackTimer::Notify()
{
    m_controls->RunPlaybackState(); 
    
    if(m_controls->GetCurrentState() == PlaybackControls::STATE_PLAY)
    {
		//std::cout << "Functions called for playback in play state! \n";
		for (auto& x: functionsNotify) {x();}
	}
    
}

void PlaybackTimer::start()
{
    wxTimer::Start(TIME_RESOLUTION,wxTIMER_CONTINUOUS); //the timer calls Notify every TIMER_INTERVAL milliseconds
}

void PlaybackTimer::AddFunctionToTimerLoop( std::function < void() > thisFunction)
{
	functionsNotify.push_back(thisFunction);
}
