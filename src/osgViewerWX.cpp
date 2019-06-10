#include "osgViewerWX.h"

bool init_listener_once = false;

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
		int width = 800;
		int height = 600;
		
		// Create the main frame window

		MainFrame *frame = new MainFrame(NULL, wxT("Binaural Audio Editor"),
			wxDefaultPosition, wxSize(width, height));

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
		
		//connect mainframe to soundproducer vector
		frame->SetSoundProducerVectorRef(&sound_producer_vector);
		
		//connect mainframe to openal soft audio engine 
		frame->SetAudioEngineReference(&audio_engine);
		
		//initialize viewer
		viewer->setSceneData(rootNode.get());
		
		//Initialize and set camera manipulator
		cameraManipulator = new osgGA::TrackballManipulator();
		viewer->setCameraManipulator(cameraManipulator);
		
		//Set camera to look at listener 10 units above origin and behind origin
		osg::Vec3d eye( 10.0, 0.0, 5.0 ); //The position of your camera -can be used to set its height position.
		osg::Vec3d center( 0.0, 0.0, 0.0 ); // The point your camera is looking at - set this to the center of the observed object.
		// The up-vector of your camera - 
		//this controls how your viewport will be rotated about its center 
		//and should be equal to [0, 0, 1] in a conventional graphics coordinate system
		osg::Vec3d up( 0.0, 0.0, 1.0 ); 

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
		
		init_listener_once = true;		
	}
}

IMPLEMENT_APP(wxOsgApp)

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
    EVT_MENU				(MainFrame::ID_CHANGE_HRTF, MainFrame::OnChangeHRTF)
    //EVT_KEY_DOWN			(MainFrame::OnKeyDown)
END_EVENT_TABLE()


/* My frame constructor */
MainFrame::MainFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
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
    
    //create and set menu bar with items file and help
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" ); //connect file menu item to bar
    menuBar->Append(menuListener, "&Listener"); //connecte listener menu item to bar
    menuBar->Append( menuSoundProducers, "&Sound Producers"); //connect Sound Producers menu item to bar
    menuBar->Append( menuHRTF, "&HRTF"); //connect HRTF menu item to bar
    menuBar->Append( menuPlayback, "&Playback"); //connect Playback menu item to bar
    menuBar->Append( menuHelp, "&Help" ); //connect help menu item  to bar

    SetMenuBar( menuBar );
    
    CreateStatusBar();
    SetStatusText( "Welcome to Binaural Audio Editor!" );
    
    //Code to initialize timeline track editor part of GUI

	TimelineFrame *timeFrame = new TimelineFrame(this); 
	

	
	m_soundproducer_track = new SoundProducerTrack("SoundProducer Track");
	m_listener_track = new ListenerTrack("Listener Track");
	
	int space = 20; //the distance,in pixels, between track and previous item(timeline or previous track)
	double start = -10.0f; //lowest value
	double end = 10.0f; //highest value
	int numTicks = 11; //number of ticks between lowest value and highest value including zero
	double resolution = 1; //the fineness of how much variable can be incremented/decremented by
	
	//initialize sound producer track stuff
	m_soundproducer_track->InitTrack(timeFrame->GetTimelineWindow(),nullptr);
	m_soundproducer_track->SetupAxisForVariable(start,end,resolution,numTicks); //setup bounds for vertical axes
	
	//initialize listener track
	m_listener_track->InitTrack(timeFrame,nullptr);
	m_listener_track->SetupAxisForVariable(start,end,resolution,numTicks);
	
	//add block of space between Timeline Ruler and Sound Producer Track
	timeFrame->AddSpacerBlock(40);
	
	//add text to indicate it is a Listener Track
	timeFrame->AddText("Listener Track",wxPoint(40,90));
	
	//add x,y,z tracks of ListenerTrack to time frame
	timeFrame->AddTrack(m_listener_track->GetReferenceToXTrack(),space);
	timeFrame->AddText("X:",wxPoint(20,700));
	timeFrame->AddTrack(m_listener_track->GetReferenceToYTrack(),space);
	timeFrame->AddText("Y:",wxPoint(20,850));
	timeFrame->AddTrack(m_listener_track->GetReferenceToZTrack(),space);
	timeFrame->AddText("Z:",wxPoint(20,1000));
	
	//add special soundproducertrack function to call during playback
	//it will also call x,y,z track playback functions
	timeFrame->AddTrackFunctionToCallInTimerLoopPlayState(m_soundproducer_track); 
	timeFrame->AddTrackFunctionToCallInTimerLoopPlayState(m_listener_track); 
	
	//add block of space between Sound Producer Track and Listener Track
	timeFrame->AddSpacerBlock(40);
	
	//add text to indicate it is a Sound Producer Track
	timeFrame->AddText("Sound Producer Track",wxPoint(40,600));
	
	//add x,y,z tracks of SoundProducerTrack to time frame
	timeFrame->AddTrack(m_soundproducer_track->GetReferenceToXTrack(),space);
	timeFrame->AddText("X:",wxPoint(20,200));
	timeFrame->AddTrack(m_soundproducer_track->GetReferenceToYTrack(),space);
	timeFrame->AddText("Y:",wxPoint(20,350));
	timeFrame->AddTrack(m_soundproducer_track->GetReferenceToZTrack(),space);
	timeFrame->AddText("Z:",wxPoint(20,500));
	
	//initialize button that adds soundproducer track
	//m_add_soundproducertrack_button = new wxButton(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Add SoundProducer Track"), wxPoint(20,100), wxSize(110, 30) );
	//m_add_soundproducertrack_button->Bind(wxEVT_BUTTON, &MainFrame::OnAddSoundProducerTrack,this);
	
	//m_remove_soundproducertrack_button = new wxButton(timeFrame->GetTimelineWindow(), wxID_ANY, wxT("Remove SoundProducer Track"), wxPoint(20,200), wxSize(110, 30) );
	//m_remove_soundproducertrack_button->Bind(wxEVT_BUTTON, &MainFrame::OnRemoveSoundProducerTrack,this);
	
	m_soundproducer_track->Show(); //show the track
	m_listener_track->Show();
	timeFrame->Show(true); //show the timeframe
	
}

void MainFrame::SetViewer(osgViewer::Viewer *viewer){_viewer = viewer;}

void MainFrame::SetRootNode(osg::Group *root){_rootNode = root;}

void MainFrame::SetSoundProducerVectorRef(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector)
{
	sound_producer_vector_ref = sound_producer_vector;
	m_soundproducer_track->SetReferenceToSoundProducerVector(sound_producer_vector);
}

void MainFrame::SetListenerReference(Listener* thisListener)
{ 
	listenerPtr = thisListener;
	m_listener_track->SetReferenceToListenerToManipulate(listenerPtr);
}

void MainFrame::SetAudioEngineReference(OpenAlSoftAudioEngine* audioEngine){ audioEnginePtr = audioEngine;}

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
	std::unique_ptr <SoundProducer> thisSoundProducer( new SoundProducer() );
	
	thisSoundProducer->InitSoundProducer(name,filePath,buffer,x,y,z);
	
	//add position attitude transform to root group of nodes
	_rootNode->addChild( thisSoundProducer.get()->getTransformNode() );
	
	//move sound producer unique pointer to sound producer vector of unique pointers
	sound_producer_vector_ref->push_back(std::move(thisSoundProducer));
	
	m_soundproducer_track->AddRecentSoundProducerMadeToTrack(); 
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
	wxMessageBox( "To change HRTF, \n 1. Open a terminal/console. \n 2. Type alsoft-config into the terminal/console and hit the Enter key.\n 3. Click on the HRTF tab in the OpenAL Soft Configuration window. \n 4. Select the HRTF in Preferred HRTF box.\n 5. Put a check mark on the Force On box. \n 6. Close Binaural Audio Editor and reopen it.",
                  "Change HRTF", wxOK | wxICON_INFORMATION );
}

void MainFrame::OnEditListener(wxCommandEvent& event)
{
	if(listenerPtr == nullptr){std::cout << "Listener Raw Pointer is null in OnEditListener. \n";}
	
	std::unique_ptr <EditListenerDialog> editListenerDialog(new EditListenerDialog( wxT("Edit Listener"),
																		listenerPtr)
															);
																				
    editListenerDialog->Show(true);
}

void MainFrame::OnAddSoundProducerTrack(wxCommandEvent& event)
{
	
}

void MainFrame::OnRemoveSoundProducerTrack(wxCommandEvent& event)
{
	
}

void MainFrame::OnPopupClick(wxCommandEvent& evt)
{
	void *data=static_cast<wxMenu *>( evt.GetEventObject() )->GetClientData();
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
	
	//std::cout << "keydown in OSGCanvas\n";
	
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
