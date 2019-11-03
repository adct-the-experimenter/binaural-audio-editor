// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifdef WIN32
#include <winsock2.h>
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "osgViewerWX.h"

bool init_listener_once = false;

wxOsgApp::wxOsgApp()
{
	m_listener_reverb_thread = nullptr;
}

wxOsgApp::~wxOsgApp()
{
    
    //Free CheckListenerReverbThread member
	
	{
		wxCriticalSectionLocker enter(m_ReverbThreadCS);
		if (m_listener_reverb_thread)         // does the thread still exist?
		{
			wxMessageOutputDebug().Printf("wxOsgApp: deleting listener reverb thread");
			
			if (m_listener_reverb_thread->Delete() != wxTHREAD_NO_ERROR )
			{
				wxLogError("Can't delete the thread!");
			}
				
		}
    }       
    // exit from the critical section to give the thread
    // the possibility to enter its destructor
    // (which is guarded with m_pThreadCS critical section!)
    
    while (1)
    {
        { // was the ~MyThread() function executed?
            wxCriticalSectionLocker enter(m_ReverbThreadCS);
            if (!m_listener_reverb_thread){break;}
        }
        
        // wait for thread completion
        wxThread::This()->Sleep(1);
    }
				
	
}

CheckListenerReverbZoneThread::CheckListenerReverbZoneThread(EffectsManager* manager,wxOsgApp* handler)
{
	m_effects_manager_ptr = manager;
	
	m_ThreadHandler = handler;
	
	//intialize thread outside of class
	/*
		 CheckListenerReverbZoneThread *thread = new CheckListenerReverbZoneThread(); 
		 if ( thread->Create() != wxTHREAD_NO_ERROR ) 
		 {
			wxLogError(wxT("Can't create thread!")); 
		 } 
	*/
	
	//call wxThread:: Run virtual function to start thread which runs Entry()
	
	//call wxThRead::Delete to destroy thread
}

CheckListenerReverbZoneThread::~CheckListenerReverbZoneThread()
{
	wxCriticalSectionLocker enter(m_ThreadHandler->m_ReverbThreadCS);
    // the thread is being destroyed; make sure not to leave dangling pointers around
    m_ThreadHandler->m_listener_reverb_thread = NULL;
}

wxThread::ExitCode CheckListenerReverbZoneThread::Entry() 
{

	while (!TestDestroy() )
	{
		wxMilliSleep(250); //sleep for 500 milliseconds
		
		m_effects_manager_ptr->PerformReverbThreadOperation();
		
	}
	
	return nullptr;  
}

// `Main program' equivalent, creating windows and returning main app frame
bool wxOsgApp::OnInit()
{
	/*
    if (argc<2)
    {
        std::cout << wxString(argv[0]).mb_str() <<": requires filename argument." << std::endl;
        return false;
    }
	*/

    //initialize openalsoft audio engine class
    if(!audio_engine.initOpenALSoft())
    {
		std::cout << "Fatal error. Failed to initialize OpenAL Soft! \n";
		return false;
	}
	else
	{
		//initialize audio stuff


		//initialize graphical stuff
		int width = 800;
		int height = 600;

		// Create the main frame window

		MainFrame *frame = new MainFrame(NULL, wxT("Binaural Audio Editor"),
			wxDefaultPosition, wxSize(width, height),&audio_engine);

		// create osg canvas
		//    - initialize

		int attributes[7];
		attributes[0] = int(WX_GL_DOUBLEBUFFER);
		attributes[1] = WX_GL_RGBA;
		attributes[2] = WX_GL_DEPTH_SIZE;
		attributes[3] = 8;
		attributes[4] = WX_GL_STENCIL_SIZE;
		attributes[5] = 8;
		attributes[6] = 0;

		OSGCanvas *canvas = new OSGCanvas(frame, wxID_ANY, wxDefaultPosition,
			wxSize(width, height), wxSUNKEN_BORDER, wxT("osgviewerWX"), attributes);

		//set function KeyDownLogic to be called every time key pressed event happens in OSGCanvas
		using std::placeholders::_1;
		std::function<void(int&)> func = std::bind( &wxOsgApp::KeyDownLogic, this, _1 );

		canvas->SetReferenceToFunctionToRunKeydown(func);

		GraphicsWindowWX* gw = new GraphicsWindowWX(canvas);

		canvas->SetGraphicsWindow(gw);

		osgViewer::Viewer *viewer = new osgViewer::Viewer;
		viewer->getCamera()->setGraphicsContext(gw);
		viewer->getCamera()->setViewport(0,0,width,height);

		// set the draw and read buffers up for a double buffered window with rendering going to back buffer
		viewer->getCamera()->setDrawBuffer(GL_BACK);
		viewer->getCamera()->setReadBuffer(GL_BACK);

		//viewer->addEventHandler(new osgViewer::StatsHandler);
		viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);



		//init geommetry node which is a leaf node of scenegraph
		//containing geometry information
		rootNode = new osg::Group;
		frame->SetRootNode(rootNode);

		wxOsgApp::initListener();

		//connect mainframe to listener
		frame->SetListenerReference(listener.get());

		//connect mainframe to external listener
		frame->SetListenerExternalReference(listenerExternal.get());

		//connect mainframe to soundproducer vector
		frame->SetSoundProducerVectorRef(&sound_producer_vector);

		//connect mainframe to openal soft audio engine
		frame->SetAudioEngineReference(&audio_engine);
		
		//initialize effects manager
		effects_manager_ptr = std::unique_ptr <EffectsManager>( new EffectsManager( frame->GetReferenceToSoundProducerTrackManager(), listener.get() ) );
		
		 m_listener_reverb_thread = new CheckListenerReverbZoneThread(effects_manager_ptr.get(),this); 
		 if ( m_listener_reverb_thread->Create() != wxTHREAD_NO_ERROR ) 
		 {
			 wxLogError(wxT("Can't create thread!"));
			 std::cout << "Can't create thread! \n";
		 }
		 else
		 {
			 std::cout << "\nThread created! Trying to run thread.\n";
			 if(m_listener_reverb_thread->Run() != wxTHREAD_NO_ERROR)
			 {
				 wxLogError(wxT("Can't run thread!"));
				 std::cout << "Can't run thread! \n";
			 }
		 }
		 
		//connect mainframe to effects manager
		frame->SetEffectsManagerReference(effects_manager_ptr.get());
		
		//initialize viewer
		viewer->setSceneData(rootNode.get());

		//Initialize and set camera manipulator
		cameraManipulator = new osgGA::TrackballManipulator();
		viewer->setCameraManipulator(cameraManipulator);

		//Set camera to look at listener 10 units above origin and behind origin
		osg::Vec3d eye( 0.0, 10.0, 10.0 ); //The position of your camera -can be used to set its height position.
		osg::Vec3d center( 0.0, 0.0, 0.0 ); // The point your camera is looking at - set this to the center of the observed object.
		// The up-vector of your camera -
		//this controls how your viewport will be rotated about its center
		//and should be equal to [0, 1, 0] OpenAL Soft coordinate system
		osg::Vec3d up( 0.0, 1.0, 0.0 );

		cameraManipulator->setTransformation(eye,center,up);

		frame->SetViewer(viewer);

		/* Show the frame */
		frame->Show(true);
	}


    return true;
}

void wxOsgApp::initListener()
{
	if(!init_listener_once)
	{
		std::unique_ptr <Listener> thisListener( new Listener() );
		listener = std::move(thisListener);

		//std::cout << "\nListener initialized. Listener x:" << listener->getPositionX() << std::endl;

		if(listener.get() == nullptr){std::cout << "listener raw pointer is null in osgViewerWxApp init! \n";}
		else{std::cout << "\nListener raw pointer:" << listener.get() << std::endl;}

		//add position attitude transform to root node group
		rootNode->addChild(listener->getTransformNode());

		//initialize listener external
		if(listener.get() != nullptr)
		{
			std::unique_ptr <ListenerExternal> thisListenerExternal( new ListenerExternal(listener.get()) );
			listenerExternal = std::move(thisListenerExternal);
		}


		init_listener_once = true;
	}
}

void wxOsgApp::KeyDownLogic(int& thisKey)
{
	//std::cout << "KeyDown Logic called in wxOsgApp.\n";

	float distanceToMove = 1.0f;

	//if a key is pressed
	switch(thisKey)
	{
		//if w key pressed
		case 87:
		{
			if(listener){listener->MoveForward(distanceToMove);}
			break;
		}
		//if a key pressed
		case 65:
		{
			if(listener){listener->MoveLeft(distanceToMove);}
			break;
		}
		//if s key pressed
		case 83:
		{
			if(listener){listener->MoveBack(distanceToMove);}
			break;
		}
		//if d key pressed
		case 68:
		{
			if(listener){listener->MoveRight(distanceToMove);}
			break;
		}
		//if q key pressed
		case 81:
		{
			if(listener){listener->MoveDown(distanceToMove);}
			break;
		}
		//if e key pressed
		case 69:
		{
			if(listener){listener->MoveUp(distanceToMove);}
			break;
		}
		default:{break;}
	}
}

wxIMPLEMENT_APP(wxOsgApp);

//Event table for main frame specific events
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_IDLE					(MainFrame::OnIdle)
    EVT_MENU				(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU				(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU				(wxID_OPEN, MainFrame::OnOpen)
    EVT_MENU				(MainFrame::ID_CREATE_SOUND_PRODUCER, MainFrame::OnCreateSoundProducer)
    EVT_MENU				(MainFrame::ID_EDIT_MULTIPLE_SOUND_PRODUCERS, MainFrame::OnEditMultipleSoundProducers)
    EVT_BUTTON				(wxEVT_CONTEXT_MENU, MainFrame::OnPopupClick)
    EVT_MENU				(MainFrame::ID_PLAY_AUDIO, MainFrame::OnPlayAudio)
    EVT_MENU				(MainFrame::ID_TEST_HRTF, MainFrame::OnTestHRTF)
    EVT_MENU				(MainFrame::ID_LISTENER_EDIT, MainFrame::OnEditListener)
    EVT_MENU				(MainFrame::ID_SETUP_SERIAL, MainFrame::OnSetupSerial)
    EVT_MENU				(MainFrame::ID_CHANGE_HRTF, MainFrame::OnChangeHRTF)
    EVT_MENU				(MainFrame::ID_CREATE_STANDARD_REVERB_ZONE, MainFrame::OnCreateStandardReverbZone)
    EVT_MENU				(MainFrame::ID_CREATE_EAX_REVERB_ZONE, MainFrame::OnCreateEAXReverbZone)
    //EVT_KEY_DOWN			(MainFrame::OnKeyDown)
END_EVENT_TABLE()


/* My frame constructor */
MainFrame::MainFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, OpenAlSoftAudioEngine* thisAudioEngine,long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
	MainFrame::SetAudioEngineReference(thisAudioEngine);

	//create file menu item
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);

    //create help menu item
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    //create sound producers menu item
    wxMenu* menuSoundProducers = new wxMenu;
    menuSoundProducers->Append(MainFrame::ID_CREATE_SOUND_PRODUCER,"&Create Sound Producer");

    //create the edit multiple sound producers menu item
    menuSoundProducers->Append(MainFrame::ID_EDIT_MULTIPLE_SOUND_PRODUCERS,"&Edit Sound Producers");

    //create playback menu item
    wxMenu* menuPlayback = new wxMenu;
    menuPlayback->Append(MainFrame::ID_PLAY_AUDIO,"&Play Audio");

    //Create hrtf menu item
    wxMenu* menuHRTF = new wxMenu;
    menuHRTF->Append(MainFrame::ID_TEST_HRTF,"&Test HRTF");
    menuHRTF->Append(MainFrame::ID_CHANGE_HRTF, "&Change HRTF");

    //create listener menu item
    wxMenu* menuListener = new wxMenu;
    menuListener->Append(MainFrame::ID_LISTENER_EDIT,"&Edit Listener");
    menuListener->Append(MainFrame::ID_SETUP_SERIAL,"&Setup Serial");
    
    //create effects menu items
	wxMenu* menuEffects = new wxMenu;
    menuEffects->Append(MainFrame::ID_CREATE_STANDARD_REVERB_ZONE,"&Create Standard Reverb Zone");
    menuEffects->Append(MainFrame::ID_CREATE_EAX_REVERB_ZONE,"&Create EAX Reverb Zone");
    
    //create and set menu bar with items file and help
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" ); //connect file menu item to bar
    menuBar->Append(menuListener, "&Listener"); //connecte listener menu item to bar
    menuBar->Append( menuSoundProducers, "&Sound Producers"); //connect Sound Producers menu item to bar
    menuBar->Append( menuEffects, "&Effects");
    menuBar->Append( menuHRTF, "&HRTF"); //connect HRTF menu item to bar	
    menuBar->Append( menuHelp, "&Help" ); //connect help menu item  to bar

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( "Welcome to Binaural Audio Editor!" );

    //Code to initialize timeline track editor part of GUI

	timeFrame = new TimelineFrame(this);


	soundproducertrack_manager_ptr = std::unique_ptr <SoundProducerTrackManager>(new SoundProducerTrackManager("SoundProducer Track Manager",
																		audioEnginePtr->GetReferenceToAudioDevice(),
																		audioEnginePtr->GetReferenceToAudioContext() ) ) ;

	soundproducertrack_manager_ptr->SetReferenceToSoundProducerTrackVector(&m_soundproducer_track_vec);



	m_soundproducer_track_vec.push_back(new SoundProducerTrack("SoundProducer Track",
											audioEnginePtr->GetReferenceToAudioDevice(),
											audioEnginePtr->GetReferenceToAudioContext())
										);
	soundproducertrack_manager_ptr->AddSourceOfLastTrackToSoundProducerTrackManager();

	m_listener_track = new ListenerTrack("Listener Track");

	int space = 20; //the distance,in pixels, between track and previous item(timeline or previous track)
	double start = -10.0f; //lowest value
	double end = 10.0f; //highest value
	int numTicks = 11; //number of ticks between lowest value and highest value including zero
	double resolution = 1; //the fineness of how much variable can be incremented/decremented by

	double ostart = -1;
	double oend = 1;
	double oresolution = 0.1;

	//initialize listener track
	m_listener_track->InitTrack(timeFrame,nullptr);
	m_listener_track->SetupAxisForPositionVariable(start,end,resolution,numTicks);

	m_listener_track->SetupAxisForOrientationVariable(ostart,oend,oresolution,numTicks);

	//add block of space between Timeline Ruler and Sound Producer Track
	timeFrame->AddSpacerBlock(40);

	//add text to indicate it is a Listener Track
	wxBoxSizer* hboxTextListener = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textListener = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener Track"),wxDefaultPosition );
	hboxTextListener->Add(textListener);
	timeFrame->AddBoxSizer(hboxTextListener);

	//add x,y,z tracks of ListenerTrack to time frame

	//add label for x track
	wxBoxSizer* hboxLXText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textLX = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener X Position"),wxDefaultPosition );
	hboxLXText->Add(textLX);
	timeFrame->AddBoxSizer(hboxLXText);

	timeFrame->AddTrack(m_listener_track->GetReferenceToXTrack(),space);

	//add label for y track
	wxBoxSizer* hboxLYText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textLY = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener Y Position"),wxDefaultPosition );
	hboxLYText->Add(textLY);
	timeFrame->AddBoxSizer(hboxLYText);

	timeFrame->AddTrack(m_listener_track->GetReferenceToYTrack(),space);

	//add label for z track
	wxBoxSizer* hboxLZText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textLZ = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener Z Position"),wxDefaultPosition );
	hboxLZText->Add(textLZ);
	timeFrame->AddBoxSizer(hboxLZText);

	timeFrame->AddTrack(m_listener_track->GetReferenceToZTrack(),space);

	//add quat w,x,y,z tracks of ListenerTrack to time frame

	//add label for w quat track
	wxBoxSizer* hboxLQWText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textLQW = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener Quaternion W"),wxDefaultPosition );
	hboxLQWText->Add(textLQW);
	timeFrame->AddBoxSizer(hboxLQWText);

	timeFrame->AddTrack(m_listener_track->GetReferenceToQuatWTrack(),space);

	//add label for x quat track
	wxBoxSizer* hboxLQXText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textLQX = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener Quaternion X"),wxDefaultPosition );
	hboxLQXText->Add(textLQX);
	timeFrame->AddBoxSizer(hboxLQXText);

	timeFrame->AddTrack(m_listener_track->GetReferenceToQuatXTrack(),space);

	//add label for y quat track
	wxBoxSizer* hboxLQYText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textLQY = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener Quaternion Y"),wxDefaultPosition );
	hboxLQYText->Add(textLQY);
	timeFrame->AddBoxSizer(hboxLQYText);

	timeFrame->AddTrack(m_listener_track->GetReferenceToQuatYTrack(),space);

	//add label for z quat track
	wxBoxSizer* hboxLQZText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textLQZ = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Listener Quaternion Z"),wxDefaultPosition );
	hboxLQZText->Add(textLQZ);
	timeFrame->AddBoxSizer(hboxLQZText);

	timeFrame->AddTrack(m_listener_track->GetReferenceToQuatZTrack(),space);


	timeFrame->AddTrackFunctionToCallInTimerLoopPlayState(m_listener_track);

	//add block of space between Sound Producer Track and Listener Track
	timeFrame->AddSpacerBlock(40);

//Initialize SoundProducer Track

	//initialize sound producer track stuff
	m_soundproducer_track_vec[0]->InitTrack(timeFrame->GetTimelineWindow(),nullptr);

	//initialize audio track
	wxButton* browseButton = new wxButton(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Browse"), wxDefaultPosition, wxSize(70, 30) );
	m_soundproducer_track_vec[0]->GetReferenceToStereoAudioTrack()->SetReferenceToBrowseButton(browseButton);
	m_soundproducer_track_vec[0]->GetReferenceToStereoAudioTrack()->InitTrack(timeFrame->GetTimelineWindow(),nullptr);
	m_soundproducer_track_vec.at(0)->SetReferenceToPlaybackControls(timeFrame->GetPlaybackControlsReference());

	//initialize double tracks
	m_soundproducer_track_vec[0]->SetupAxisForVariable(start,end,resolution,numTicks); //setup bounds for vertical axes
	m_soundproducer_track_vec[0]->SetReferenceToSoundProducerRegistry(&soundproducer_registry);
	m_soundproducer_track_vec[0]->UpdateComboBoxListFromSoundProducerRegistry();
	soundproducer_registry.AddReferenceToComboBox(m_soundproducer_track_vec[0]->GetReferenceToComboBox());

	//add text to indicate it is a Sound Producer Track
	wxBoxSizer* hboxTextSPTrack = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textSPTrack = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Sound Producer Track(x,y,z)"),wxDefaultPosition );
	hboxTextSPTrack->Add(textSPTrack);
	hboxTextSPTrack->Add(m_soundproducer_track_vec[0]->GetReferenceToComboBox());
	timeFrame->AddBoxSizer(hboxTextSPTrack);

	sound_producer_track_count = 1;

	double audio_start = 0.0f; //lowest value
	double audio_end = 10.0f; //highest value
	int audio_numTicks = 11; //number of ticks between lowest value and highest value including zero
	double audio_resolution = 0.1; //the fineness of how much variable can be incremented/decremented by

	//setup bounds for vertical axis
	m_soundproducer_track_vec[0]->SetupAxisForAudio(audio_start,audio_end,audio_resolution,audio_numTicks);

	timeFrame->AddSpacerBlock(50);

	//setup browse button and text label for stereo audio track
	wxBoxSizer* hboxButtonText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textButtonText = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Stereo Audio Track"),wxDefaultPosition );
	hboxButtonText->Add(textButtonText);
	hboxButtonText->Add(browseButton);
	timeFrame->AddBoxSizer(hboxButtonText);

	//add label for left channel audio
	wxBoxSizer* hboxAudioLeftText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textAudioLeft = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Left Channel Audio"),wxDefaultPosition );
	hboxAudioLeftText->Add(textAudioLeft);
	timeFrame->AddBoxSizer(hboxAudioLeftText);

	//add left channel track to time frame
	timeFrame->AddTrack(m_soundproducer_track_vec[0]->GetReferenceToStereoAudioTrack()->GetReferenceToLeftChannelTrack(),space);

	//add label for right channel audio
	wxBoxSizer* hboxAudioRightText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textAudioRight = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Right Channel Audio"),wxDefaultPosition );
	hboxAudioRightText->Add(textAudioRight);
	timeFrame->AddBoxSizer(hboxAudioRightText);

	//add right channel track to time frame
	timeFrame->AddTrack(m_soundproducer_track_vec[0]->GetReferenceToStereoAudioTrack()->GetReferenceToRightChannelTrack(),space);

	timeFrame->AddTrackFunctionToCallInTimerLoopPlayState(soundproducertrack_manager_ptr.get());
	timeFrame->AddTrackFunctionToCallInTimerLoopNullState(soundproducertrack_manager_ptr.get());
	timeFrame->AddTrackFunctionToCallInTimerLoopPauseState(soundproducertrack_manager_ptr.get());
	timeFrame->AddTrackFunctionToCallInTimerLoopRewindState(soundproducertrack_manager_ptr.get());
	timeFrame->AddTrackFunctionToCallInTimerLoopFastForwardState(soundproducertrack_manager_ptr.get());


	//add x,y,z tracks of SoundProducerTrack to time frame

	//add label for x track
	wxBoxSizer* hboxXText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textX = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("X"),wxDefaultPosition );
	hboxXText->Add(textX);
	timeFrame->AddBoxSizer(hboxXText);

	timeFrame->AddTrack(m_soundproducer_track_vec[0]->GetReferenceToXTrack(),space);

	//add label for y track
	wxBoxSizer* hboxYText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textY = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Y"),wxDefaultPosition );
	hboxYText->Add(textY);
	timeFrame->AddBoxSizer(hboxYText);

	timeFrame->AddTrack(m_soundproducer_track_vec[0]->GetReferenceToYTrack(),space);

	//add label for z track
	wxBoxSizer* hboxZText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textZ = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Z"),wxDefaultPosition );
	hboxZText->Add(textZ);
	timeFrame->AddBoxSizer(hboxZText);

	timeFrame->AddTrack(m_soundproducer_track_vec[0]->GetReferenceToZTrack(),space);

	//add block of space between Sound Producer Track and bottom of window, also extends height of window
	timeFrame->AddSpacerBlock(100);

	//intialize add and remove buttons for new soundproducer tracks
	m_add_soundproducertrack_button = new wxButton(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Add SoundProducer Track"), wxDefaultPosition, wxSize(200, 30) );
	m_add_soundproducertrack_button->Bind(wxEVT_BUTTON, &MainFrame::OnAddSoundProducerTrack,this);


	m_remove_soundproducertrack_button = new wxButton(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Remove SoundProducer Track"), wxDefaultPosition, wxSize(200, 30) );
	m_remove_soundproducertrack_button->Bind(wxEVT_BUTTON, &MainFrame::OnRemoveSoundProducerTrack,this);

	m_add_rm_box_sizer = new wxBoxSizer(wxVERTICAL);

	m_add_rm_box_sizer->Add(m_remove_soundproducertrack_button);
	m_add_rm_box_sizer->AddSpacer(25);
	m_add_rm_box_sizer->Add(m_add_soundproducertrack_button);

	timeFrame->AddBoxSizer(m_add_rm_box_sizer);

	timeFrame->Show(true); //show the timeframe

}

void MainFrame::SetViewer(osgViewer::Viewer *viewer){_viewer = viewer;}

void MainFrame::SetRootNode(osg::Group *root){_rootNode = root;}

void MainFrame::SetSoundProducerVectorRef(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector)
{
	sound_producer_vector_ref = sound_producer_vector;

	soundproducer_registry.SetReferenceToSoundProducerVector(sound_producer_vector_ref);
}

void MainFrame::SetListenerReference(Listener* thisListener)
{
	listenerPtr = thisListener;
	m_listener_track->SetReferenceToListenerToManipulate(listenerPtr);
}

void MainFrame::SetListenerExternalReference(ListenerExternal* thisListenerExternal)
{
	listenerExternalPtr = thisListenerExternal;
	m_listener_track->SetReferenceToExternalListener(thisListenerExternal);
}

void MainFrame::SetAudioEngineReference(OpenAlSoftAudioEngine* audioEngine){ audioEnginePtr = audioEngine;}

void MainFrame::SetEffectsManagerReference(EffectsManager* effectsManager){effectsManagerPtr = effectsManager;}

SoundProducerTrackManager* MainFrame::GetReferenceToSoundProducerTrackManager(){return soundproducertrack_manager_ptr.get();}

void MainFrame::OnIdle(wxIdleEvent &event)
{
    if (!_viewer->isRealized())
        return;

    _viewer->frame();

    event.RequestMore();

    //std::cout << "App is Idle." << std::endl;
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close( true ); //close window
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	//show message box with ok icon,
	//window title:About Binaural Audio Editor
	//message:
    wxMessageBox( "Version 0.0 of Binaural Audio Editor. \n This program is for producing and editing binaural audio from mono or stereo samples of audio.",
                  "About Binaural Audio Editor", wxOK | wxICON_INFORMATION );
}

void MainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{

	wxFileDialog fileDlg(this, _("Choose the WAV file"), wxEmptyString, wxEmptyString, _("WAV file|*.wav|All files|*.*"));
	if (fileDlg.ShowModal() == wxID_OK)
	{
		wxString path = fileDlg.GetPath();
		//use this path in your app
	}

}

void MainFrame::OnPlayAudio(wxCommandEvent& event)
{
	if(sound_producer_vector_ref != nullptr)
	{
		//play all sounds of sound producers at same time for now
		for(size_t i = 0; i < sound_producer_vector_ref->size(); i++)
		{
			SoundProducer* thisSoundProducer = sound_producer_vector_ref->at(i).get();
			audioEnginePtr->playSound( thisSoundProducer->getSource() );
		}
	}
}

void MainFrame::OnCreateSoundProducer(wxCommandEvent& event)
{
	//show message box with ok icon,
	//window title:about hello world
	//message: This is a wxWidgets Helo world sample
    //wxMessageBox( "Create Sound Producer", "Create Sound Producer",wxOK | wxCANCEL |wxICON_INFORMATION );

    std::unique_ptr <CreateSoundProducerDialog> soundProducerNewDialog(new CreateSoundProducerDialog(wxT("Create New Sound Producer"),
																									audioEnginePtr) );
    soundProducerNewDialog->Show(true);

    if(soundProducerNewDialog->OkClickedOn())
    {
		double x,y,z;

		soundProducerNewDialog->getNewPosition(x,y,z);
		std::string name = soundProducerNewDialog->getNewName();
		std::string filePath = soundProducerNewDialog->getSoundFilePath();
		ALuint buffer = soundProducerNewDialog->getBuffer();
		MainFrame::CreateSoundProducer(name,filePath,buffer,x,y,z);
	}

}

void MainFrame::CreateSoundProducer(std::string& name, std::string& filePath, ALuint& buffer,double& x, double& y, double& z)
{

	sound_producer_vector_ref->push_back( std::unique_ptr <SoundProducer>(new SoundProducer()) );

	sound_producer_vector_ref->back()->InitSoundProducer(name,filePath,buffer,x,y,z);

	//add position attitude transform to root group of nodes
	_rootNode->addChild( sound_producer_vector_ref->back()->getTransformNode() );

	soundproducer_registry.AddRecentSoundProducerMadeToRegistry();

	soundproducer_registry.UpdateAllComboBoxesList();
}

void MainFrame::OnEditMultipleSoundProducers(wxCommandEvent& event)
{
	std::unique_ptr <EditMultipleSoundProducersDialog> soundProducerEditDialog(new EditMultipleSoundProducersDialog( wxT("Edit Sound Producers"),
																													sound_producer_vector_ref,
																													audioEnginePtr)
																				);

    soundProducerEditDialog->Show(true);

}

void MainFrame::OnTestHRTF(wxCommandEvent& event)
{
	std::unique_ptr <HRTFTestDialog> hrtfTestDialog(new HRTFTestDialog( wxT("Test HRTF"),
																		audioEnginePtr)
													);

    hrtfTestDialog->Show(true);

}

void MainFrame::OnChangeHRTF(wxCommandEvent& event)
{
	std::unique_ptr <ChangeHRTFDialog> hrtfChangeDialog(new ChangeHRTFDialog( wxT("Change HRTF"),
																		audioEnginePtr)
													);

    hrtfChangeDialog->Show(true);
}

void MainFrame::OnEditListener(wxCommandEvent& event)
{
	if(listenerPtr == nullptr){std::cout << "Listener Raw Pointer is null in OnEditListener. \n";}

	std::unique_ptr <EditListenerDialog> editListenerDialog(new EditListenerDialog( wxT("Edit Listener"),
																		listenerPtr)
															);

    editListenerDialog->Show(true);
}

void MainFrame::OnSetupSerial(wxCommandEvent& event)
{
	if(listenerPtr == nullptr){std::cout << "Listener Raw Pointer is null in OnSetupSerial. \n";}

	std::unique_ptr <SetupSerialDialog> setupSerialDialog(new SetupSerialDialog( wxT("Setup Serial"),
																		listenerExternalPtr)
															);

    setupSerialDialog->Show(true);
}

void MainFrame::OnCreateStandardReverbZone(wxCommandEvent& event)
{
	//show message box with ok icon,
	//window title:about hello world
	//message: This is a wxWidgets Helo world sample
    //wxMessageBox( "Create Sound Producer", "Create Sound Producer",wxOK | wxCANCEL |wxICON_INFORMATION );

    std::unique_ptr <CreateStandardReverbZoneDialog> reverbZoneNewDialog(new CreateStandardReverbZoneDialog(wxT("Create New Standard Reverb Zone"),
																									effectsManagerPtr) );
    reverbZoneNewDialog->Show(true);

    if(reverbZoneNewDialog->OkClicked())
    {
		double x,y,z,width;
		ReverbStandardProperties properties;
		
		reverbZoneNewDialog->getNewPosition(x,y,z);
		std::string name = reverbZoneNewDialog->getNewName();
		width = reverbZoneNewDialog->getNewWidth();
		properties = reverbZoneNewDialog->getNewProperties();
		
		effectsManagerPtr->CreateStandardReverbZone(name,x,y,z,width,properties);
		
		//add position attitude transform to root group of nodes
		_rootNode->addChild( (effectsManagerPtr->GetReferenceToReverbZoneVector())->back().getTransformNode() );
	}

}

void MainFrame::OnCreateEAXReverbZone(wxCommandEvent& event)
{
	//show message box with ok icon,
	//window title:about hello world
	//message: This is a wxWidgets Helo world sample
    //wxMessageBox( "Create Sound Producer", "Create Sound Producer",wxOK | wxCANCEL |wxICON_INFORMATION );

    std::unique_ptr <CreateEAXReverbZoneDialog> reverbZoneNewDialog(new CreateEAXReverbZoneDialog(wxT("Create New EAX Reverb Zone"),
																									effectsManagerPtr) );
    reverbZoneNewDialog->Show(true);

    if(reverbZoneNewDialog->OkClicked())
    {
		double x,y,z,width;
		ReverbEAXProperties properties;
		
		reverbZoneNewDialog->getNewPosition(x,y,z);
		std::string name = reverbZoneNewDialog->getNewName();
		width = reverbZoneNewDialog->getNewWidth();
		properties = reverbZoneNewDialog->getNewProperties();
		
		effectsManagerPtr->CreateEAXReverbZone(name,x,y,z,width,properties);
		
		//add position attitude transform to root group of nodes
		_rootNode->addChild( (effectsManagerPtr->GetReferenceToReverbZoneVector())->back().getTransformNode() );
	}

}

void MainFrame::OnEditMultipleReverbZones(wxCommandEvent& event)
{
	std::unique_ptr <EditMultipleReverbZonesDialog> reverbZoneEditDialog(new EditMultipleReverbZonesDialog( wxT("Edit Reverb Zones"),
																													effectsManagerPtr));
																				

    reverbZoneEditDialog->Show(true);

}

void MainFrame::OnAddSoundProducerTrack(wxCommandEvent& event)
{
	//move remove sound producer track button to top of new sound producer track

	//detach sizer containing add/rm soundproducertrack buttons from window, add it back later
	timeFrame->GetTimelineWindow()->GetSizer()->Detach(m_add_rm_box_sizer);

	//create new sound producer track and add it to vector of sound producer tracks in MainFrame
	MainFrame::CreateNewSoundProducerTrack();

	//move add sound producer track button to bottom of new sound producer track

	//add sizer containing add/rm soundproducer track buttons to bottom of window
	timeFrame->GetTimelineWindow()->GetSizer()->Add(m_add_rm_box_sizer);

	timeFrame->Layout();

}

void MainFrame::CreateNewSoundProducerTrack()
{
	std::cout << "Count of items in timeline window size:" << timeFrame->GetTimelineWindow()->GetSizer()->GetItemCount() << std::endl;
	//set title based on how many sound producer tracks there are
	sound_producer_track_count += 1;

	//convert number to string
	std::string result;          // string which will contain the result
	std::ostringstream convert;   // stream used for the conversion
	convert << sound_producer_track_count;      // insert the textual representation of 'Number' in the characters in the stream
	result = convert.str(); // set 'Result' to the contents of the stream

	wxString title = wxString("SoundProducer Track " + result);

	m_soundproducer_track_vec.push_back(new SoundProducerTrack(title,
											audioEnginePtr->GetReferenceToAudioDevice(),
											audioEnginePtr->GetReferenceToAudioContext())
										);
	soundproducertrack_manager_ptr->AddSourceOfLastTrackToSoundProducerTrackManager();

	//initialize sound producer track stuff
	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->InitTrack(timeFrame->GetTimelineWindow(),nullptr);
	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->SetReferenceToPlaybackControls(timeFrame->GetPlaybackControlsReference());

	//initialize audio track
	wxButton* browseButton = new wxButton(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Browse"), wxDefaultPosition, wxSize(70, 30) );
	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToStereoAudioTrack()->SetReferenceToBrowseButton(browseButton);
	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToStereoAudioTrack()->InitTrack(timeFrame->GetTimelineWindow(),nullptr);

	//initialize double tracks

	int space = 20; //the distance,in pixels, between track and previous item(timeline or previous track)
	double start = -10.0f; //lowest value
	double end = 10.0f; //highest value
	int numTicks = 11; //number of ticks between lowest value and highest value including zero
	double resolution = 1; //the fineness of how much variable can be incremented/decremented by

	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->SetupAxisForVariable(start,end,resolution,numTicks); //setup bounds for vertical axes
	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->SetReferenceToSoundProducerRegistry(&soundproducer_registry);
	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->UpdateComboBoxListFromSoundProducerRegistry();
	soundproducer_registry.AddReferenceToComboBox(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToComboBox());

	//add text to indicate it is a Sound Producer Track
	wxBoxSizer* hboxTextSPTrack = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textSPTrack = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Sound Producer Track(x,y,z)"),wxDefaultPosition );
	hboxTextSPTrack->Add(textSPTrack);
	hboxTextSPTrack->Add(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToComboBox());
	timeFrame->AddBoxSizer(hboxTextSPTrack);



	double audio_start = 0.0f; //lowest value
	double audio_end = 10.0f; //highest value
	int audio_numTicks = 11; //number of ticks between lowest value and highest value including zero
	double audio_resolution = 0.1; //the fineness of how much variable can be incremented/decremented by

	//setup bounds for vertical axis
	m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->SetupAxisForAudio(audio_start,audio_end,audio_resolution,audio_numTicks);

	timeFrame->AddSpacerBlock(50);

	//setup browse button and text label for stereo audio track
	wxBoxSizer* hboxButtonText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textButtonText = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Stereo Audio Track"),wxDefaultPosition );
	hboxButtonText->Add(textButtonText);
	hboxButtonText->Add(browseButton);
	timeFrame->AddBoxSizer(hboxButtonText);

	//add label for left channel audio
	wxBoxSizer* hboxAudioLeftText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textAudioLeft = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Left Channel Audio"),wxDefaultPosition );
	hboxAudioLeftText->Add(textAudioLeft);
	timeFrame->AddBoxSizer(hboxAudioLeftText);

	//add left channel track to time frame
	timeFrame->AddTrack(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToStereoAudioTrack()->GetReferenceToLeftChannelTrack(),space);

	//add label for right channel audio
	wxBoxSizer* hboxAudioRightText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textAudioRight = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Right Channel Audio"),wxDefaultPosition );
	hboxAudioRightText->Add(textAudioRight);
	timeFrame->AddBoxSizer(hboxAudioRightText);

	//add right channel track to time frame
	timeFrame->AddTrack(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToStereoAudioTrack()->GetReferenceToRightChannelTrack(),space);

	//timeFrame->AddTrackFunctionToCallInTimerLoopPlayState(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1));
	//timeFrame->AddTrackFunctionToCallInTimerLoopNullState(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1));
	//timeFrame->AddTrackFunctionToCallInTimerLoopPauseState(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1));
	//timeFrame->AddTrackFunctionToCallInTimerLoopRewindState(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1));
	//timeFrame->AddTrackFunctionToCallInTimerLoopFastForwardState(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1));

	//add x,y,z tracks of SoundProducerTrack to time frame

	//add label for x track
	wxBoxSizer* hboxXText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textX = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("X"),wxDefaultPosition );
	hboxXText->Add(textX);
	timeFrame->AddBoxSizer(hboxXText);

	timeFrame->AddTrack(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToXTrack(),space);

	//add label for y track
	wxBoxSizer* hboxYText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textY = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Y"),wxDefaultPosition );
	hboxYText->Add(textY);
	timeFrame->AddBoxSizer(hboxYText);

	timeFrame->AddTrack(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToYTrack(),space);

	//add label for z track
	wxBoxSizer* hboxZText = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textZ = new wxStaticText(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Z"),wxDefaultPosition );
	hboxZText->Add(textZ);
	timeFrame->AddBoxSizer(hboxZText);

	timeFrame->AddTrack(m_soundproducer_track_vec.at(m_soundproducer_track_vec.size()-1)->GetReferenceToZTrack(),space);

	//add block of space between Sound Producer Track and bottom of window, also extends height of window
	timeFrame->AddSpacerBlock(100);
}

void MainFrame::OnRemoveSoundProducerTrack(wxCommandEvent& event)
{
	size_t itemCount = 	timeFrame->GetTimelineWindow()->GetSizer()->GetItemCount();
	std::cout << "\nItem Count: " << itemCount << std::endl;

	//if item count is more than 25, which means the initial items to not be deleted are in the timeline window.
	if(itemCount > 33)
	{
		//remove x,y,z tracks of soundproducer track added to timelinewindow
		//and remove the combo box and text label
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-2);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-3);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-4);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-5);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-6);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-7);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-8);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-9);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-10);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-11);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-12);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-13);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-14);
		timeFrame->GetTimelineWindow()->GetSizer()->Remove(itemCount-15);

		//resize frame properly
		timeFrame->Layout();

		//destroy last soundproducertrack from vector containing soundproducertracks
		m_soundproducer_track_vec.pop_back();
		soundproducertrack_manager_ptr->RemoveSourceOfLastTrackFromSoundProducerTrackManager();
		soundproducer_registry.RemoveLastComboBoxReference();

		std::cout << "\nItem Count: " << timeFrame->GetTimelineWindow()->GetSizer()->GetItemCount() << std::endl;
	}

}

void MainFrame::OnPopupClick(wxCommandEvent& evt)
{
	//void *data=static_cast<wxMenu *>( evt.GetEventObject() )->GetClientData();
	//switch(evt.GetId())
	//{
	//	case ID_SOMETHING:{break;}
	//	case ID_SOMETHING_ELSE:{break;}
	//}
}

void MainFrame::OnListRightClick(wxListEvent& evt)
{
	void *data = reinterpret_cast<void *>(evt.GetItem().GetData());
	wxMenu menu;
	menu.SetClientData( data );
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnPopupClick), NULL, this);
	PopupMenu(&menu);
}

void MainFrame::OnKeyDown(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif

	std::cout << "KeyDown:" << key << std::endl;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////			///////////////////////////////////////////////////////////////////
///////////////////////OSGCanvas///////////////////////////////////////////////////////////////////
///////////////////////			///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


//OSGCanvas event table for openscenegraph specific events
BEGIN_EVENT_TABLE(OSGCanvas, wxGLCanvas)
    EVT_SIZE                (OSGCanvas::OnSize)
    EVT_PAINT               (OSGCanvas::OnPaint)
    EVT_ERASE_BACKGROUND    (OSGCanvas::OnEraseBackground)

    EVT_CHAR                (OSGCanvas::OnChar)
    EVT_KEY_UP              (OSGCanvas::OnKeyUp)
    EVT_KEY_DOWN			(OSGCanvas::OnKeyDown)
    EVT_ENTER_WINDOW        (OSGCanvas::OnMouseEnter)
    EVT_LEFT_DOWN           (OSGCanvas::OnMouseDown)
    EVT_MIDDLE_DOWN         (OSGCanvas::OnMouseDown)
    EVT_RIGHT_DOWN          (OSGCanvas::OnMouseDown)
    EVT_LEFT_UP             (OSGCanvas::OnMouseUp)
    EVT_MIDDLE_UP           (OSGCanvas::OnMouseUp)
    EVT_RIGHT_UP            (OSGCanvas::OnMouseUp)
    EVT_MOTION              (OSGCanvas::OnMouseMotion)
    EVT_MOUSEWHEEL          (OSGCanvas::OnMouseWheel)
END_EVENT_TABLE()

OSGCanvas::OSGCanvas(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *attributes)
    : wxGLCanvas(parent, id, attributes, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
    ,_context(this)
{
    // default cursor to standard
    _oldCursor = *wxSTANDARD_CURSOR;
}

OSGCanvas::~OSGCanvas()
{
}

void OSGCanvas::SetContextCurrent(){_context.SetCurrent(*this);}

void OSGCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    /* must always be here */
    wxPaintDC dc(this);
}

void OSGCanvas::OnSize(wxSizeEvent& event)
{
    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
    int width, height;
    GetClientSize(&width, &height);

    if (_graphics_window.valid())
    {
        // update the window dimensions, in case the window has been resized.
        _graphics_window->getEventQueue()->windowResize(0, 0, width, height);
        _graphics_window->resized(0,0,width,height);
    }
}

void OSGCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    /* Do nothing, to avoid flashing on MSW */
}

void OSGCanvas::OnChar(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif

    //if (_graphics_window.valid())
    //    _graphics_window->getEventQueue()->keyPress(key);

    // If this key event is not processed here, we should call
    // event.Skip() to allow processing to continue.
}

void OSGCanvas::OnKeyUp(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif


    if (_graphics_window.valid())
        _graphics_window->getEventQueue()->keyRelease(key);

    // If this key event is not processed here, we should call
    // event.Skip() to allow processing to continue.
}

void OSGCanvas::OnKeyDown(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif

	//std::cout << "keydown in OSGCanvas, key:" << key << std::endl;

	//call function to run after key pressed down
	functionToRunKeyDown(key);

    if (_graphics_window.valid())
        _graphics_window->getEventQueue()->keyRelease(key);

    // If this key event is not processed here, we should call event.skip to allow processing to continue
     event.Skip();
}

void OSGCanvas::OnMouseEnter(wxMouseEvent & /*event*/)
{
    // Set focus to ourselves, so keyboard events get directed to us
    SetFocus();
}

void OSGCanvas::OnMouseDown(wxMouseEvent &event)
{
    if (_graphics_window.valid())
    {
        _graphics_window->getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(),
            event.GetButton());
    }
}

void OSGCanvas::OnMouseUp(wxMouseEvent &event)
{
    if (_graphics_window.valid())
    {
        _graphics_window->getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(),
            event.GetButton());
    }
}

void OSGCanvas::OnMouseMotion(wxMouseEvent &event)
{
    if (_graphics_window.valid())
        _graphics_window->getEventQueue()->mouseMotion(event.GetX(), event.GetY());
}

void OSGCanvas::OnMouseWheel(wxMouseEvent &event)
{
    int delta = event.GetWheelRotation() / event.GetWheelDelta() * event.GetLinesPerAction();

    if (_graphics_window.valid()) {
        _graphics_window->getEventQueue()->mouseScroll(
            delta>0 ?
            osgGA::GUIEventAdapter::SCROLL_UP :
            osgGA::GUIEventAdapter::SCROLL_DOWN);
    }
}

void OSGCanvas::UseCursor(bool value)
{
    if (value)
    {
        // show the old cursor
        SetCursor(_oldCursor);
    }
    else
    {
        // remember the old cursor
        _oldCursor = GetCursor();

        // hide the cursor
        //    - can't find a way to do this neatly, so create a 1x1, transparent image
        wxImage image(1,1);
        image.SetMask(true);
        image.SetMaskColour(0, 0, 0);
        wxCursor cursor(image);
        SetCursor(cursor);

        // On wxGTK, only works as of version 2.7.0
        // (http://trac.wxwidgets.org/ticket/2946)
        // SetCursor( wxStockCursor( wxCURSOR_BLANK ) );
    }
}

void OSGCanvas::SetReferenceToFunctionToRunKeydown(std::function <void(int&)> thisFunction)
{
	functionToRunKeyDown = thisFunction;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////					/////////////////////////////////////////////////////////////
///////////////////////GraphicsWindowWX///////////////////////////////////////////////////////////////////
///////////////////////					////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsWindowWX::GraphicsWindowWX(OSGCanvas *canvas)
{
    _canvas = canvas;

    _traits = new GraphicsContext::Traits;

    wxPoint pos = _canvas->GetPosition();
    wxSize  size = _canvas->GetSize();

    _traits->x = pos.x;
    _traits->y = pos.y;
    _traits->width = size.x;
    _traits->height = size.y;

    init();
}

GraphicsWindowWX::~GraphicsWindowWX()
{
}

void GraphicsWindowWX::init()
{
    if (valid())
    {
        setState( new osg::State );
        getState()->setGraphicsContext(this);

        if (_traits.valid() && _traits->sharedContext.valid())
        {
            getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
            incrementContextIDUsageCount( getState()->getContextID() );
        }
        else
        {
            getState()->setContextID( osg::GraphicsContext::createNewContextID() );
        }
    }
}

void GraphicsWindowWX::grabFocus()
{
    // focus the canvas
    _canvas->SetFocus();
}

void GraphicsWindowWX::grabFocusIfPointerInWindow()
{
    // focus this window, if the pointer is in the window
    wxPoint pos = wxGetMousePosition();
    if (wxFindWindowAtPoint(pos) == _canvas)
        _canvas->SetFocus();
}

void GraphicsWindowWX::useCursor(bool cursorOn)
{
    _canvas->UseCursor(cursorOn);
}

bool GraphicsWindowWX::makeCurrentImplementation()
{
    _canvas->SetContextCurrent();
    return true;
}

void GraphicsWindowWX::swapBuffersImplementation()
{
    _canvas->SwapBuffers();
}
