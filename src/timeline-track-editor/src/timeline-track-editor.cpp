#include "timeline-track-editor.h"

//event table to out event commands to functions
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

//implements  main in MyApp(derived from wxApp)
wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
	//make new main window
	//window title: hello world
	//size: wxSize(450, 340)
	//location: wxPoint(50, 50)
    MyFrame *frame = new MyFrame( "The Name of the Application here", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    
    return true;
}



void MyApp::OnIdle(wxIdleEvent &event)
{
    std::cout << "MyApp is Idle." << std::endl;

    event.RequestMore();
    
    
}

//declare variable to change
double someVarToChange;

//declare a function to use if variable changes
void FunctionForSomeVarAfterChange()
{
	std::cout << "someVarToChange:" << someVarToChange << std::endl;
	std::cout << "function for some var change called! Do something now that double-type SomeVarToChange has changed\n";
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
	//create file menu item
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    
    //create help menu item
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    //create and  set menu bar with items file and help
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" ); //connect file menu item to file
    menuBar->Append( menuHelp, "&Help" ); //connect help menu item to help
    SetMenuBar( menuBar );
    
    CreateStatusBar();
    SetStatusText( "Welcome to the app!" );
    
    //Code to initialize timeline track editor part of GUI
    
    TimelineFrame *timeFrame = new TimelineFrame(this); 

	int space = 20; //the distance,in pixels, between track and previous item(timeline or previous track)
	DoubleTrack* track1 = new DoubleTrack("Variable Track");
	//connect double track to graphical playback controls in time frame
	track1->SetReferenceToPlaybackControls(timeFrame->GetPlaybackControlsReference());
	
	double start = -10.0f; //lowest value
	double end = 10.0f; //highest value
	int numTicks = 11; //number of ticks between lowest value and highest value including zero
	double resolution = 1; //the fineness of how much variable can be incremented/decremented by

	track1->SetupAxisForVariable(start,end,resolution,numTicks); //setup bounds for vertical axis

	//Put in the variable to change with the timeline.
	// IMPORTANT NOTE: someVarToChange must be declared outside of scope of MyFrame constructor 
	//and not go out of scope or else a segmentation fault happens
	track1->SetReferenceToVarToManipulate(&someVarToChange); 
	
	//add block of space between timeline and track
	int spaceBlockSize = 100;
	timeFrame->AddSpacerBlock(spaceBlockSize);
	
	//add text to the timeline window before track to indicate what variable the track manipulates
	wxBoxSizer* hboxText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *text = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Track for someVarToChange"),wxDefaultPosition );
	hboxText->Add(text);
	timeFrame->AddBoxSizer(hboxText);

	//set function to call after variable to manipulate has changed
	//optional
	track1->SetFunctionToCallAfterVariableChange(FunctionForSomeVarAfterChange);
	
	//add track to time frame
	timeFrame->AddTrack(track1,space);
	
	
	//FunctionToCallInXState can be changed to do whatever you want. Functions must be void and take in no parameters.
	//Can add members to DoubleTrack and reference them to get around no parameters or make own class with custom members.

	//makes DoubleTrack::FunctionToCallInPlayState() be called during play
	timeFrame->AddTrackFunctionToCallInTimerLoopPlayState(track1); 
	//makes DoubleTrack::FunctionToCallInPauseState() be called during pause
	timeFrame->AddTrackFunctionToCallInTimerLoopPauseState(track1); 
	//makes DoubleTrack::FunctionToCallInRewindState() be called during rewind
	timeFrame->AddTrackFunctionToCallInTimerLoopRewindState(track1); 
	//makes DoubleTrack::FunctionToCallInFastForwardState() be called during fast forward
	timeFrame->AddTrackFunctionToCallInTimerLoopFastForwardState(track1); 
	//makes DoubleTrack::FunctionToCallInNullState() be called when nothing is happening after stop button pressed
	timeFrame->AddTrackFunctionToCallInTimerLoopNullState(track1); 

//Initialize Audio Track

	//Initialize OpenAL Soft
	
	audioPlayer = new OpenALSoftPlayer();
	
	if(!audioPlayer->InitOpenALSoft(audioDevice,alContext))
	{
		std::cout << "Unable to make audio track because OpenAL SOft could not be initialized! \n";
	}
	else
	{
		audioPlayer->SetReferenceToAudioContext(alContext);
		audioPlayer->SetReferenceToAudioDevice(audioDevice);
		
		audioPlayer->InitBuffersForStreaming();
		
		audioPlayer->InitSource(&source);
		
		StereoAudioTrack* track2 = new StereoAudioTrack("Audio");
		track2->SetReferenceToSourceToManipulate(&source); //connect audio track to external source
		track2->SetReferenceToAudioPlayer(audioPlayer); //connect stereo audio track to audio player
		
		//connect stereo audio track to playback controls in time frame
		track2->SetReferenceToPlaybackControls(timeFrame->GetPlaybackControlsReference());
		
		wxButton* browseButton = new wxButton(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Browse"), wxDefaultPosition, wxSize(70, 30) );
		track2->SetReferenceToBrowseButton(browseButton);
		
		track2->InitTrack(timeFrame->GetTimelineWindow(),nullptr);
		
		double start = 0.0f; //lowest value
		double end = 10.0f; //highest value
		int numTicks = 11; //number of ticks between lowest value and highest value including zero
		double resolution = 0.1; //the fineness of how much variable can be incremented/decremented by

		//setup bounds for vertical axis
		track2->SetupAxisForVariable(start,end,resolution,numTicks);
		
		timeFrame->AddSpacerBlock(50);
		
		int space = 20;
		
		
		//setup browse button and text label for stereo audio track
		wxBoxSizer* hboxButtonText = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *textButtonText = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Stereo Audio Track"),wxDefaultPosition );
		hboxButtonText->Add(textButtonText);
		hboxButtonText->Add(browseButton);
		timeFrame->AddBoxSizer(hboxButtonText);
		
		//add left channel track and right channel track to time frame
		timeFrame->AddTrack(track2->GetReferenceToLeftChannelTrack(),space);
		timeFrame->AddTrack(track2->GetReferenceToRightChannelTrack(),space);
		
		timeFrame->AddTrackFunctionToCallInTimerLoopPlayState(track2);
		timeFrame->AddTrackFunctionToCallInTimerLoopNullState(track2);
		timeFrame->AddTrackFunctionToCallInTimerLoopPauseState(track2);
		timeFrame->AddTrackFunctionToCallInTimerLoopRewindState(track2);
		timeFrame->AddTrackFunctionToCallInTimerLoopFastForwardState(track2);
		
		track2->Show();
	}
	
	
	
//Show Tracks and Time Frame

	timeFrame->Show(true); //show the timeframe
		
}




void MyFrame::OnExit(wxCommandEvent& event)
{
	audioPlayer->CloseOpenALSoft(audioDevice,alContext);
    Close( true ); //close window
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
	//show message box with ok icon, 
	//window title:about hello world
	//message: This is a wxWidgets Helo world sample
    wxMessageBox( "This is a sample to use for your project",
                  "About the Project", wxOK | wxICON_INFORMATION );
}

void MyFrame::OnHello(wxCommandEvent& event)
{
	//show message
    wxLogMessage("Hello world!");
}


