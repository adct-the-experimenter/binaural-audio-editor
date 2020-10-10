#include "main_app.h"

//event table to out event commands to functions
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

//implements  main in MyApp(derived from wxApp)
wxIMPLEMENT_APP(MyApp);

#define __GXX_ABI_VERSION 1013

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
    menuBar->Append( menuHelp, "&Help" ); //connect help menu item to help
    SetMenuBar( menuBar );
    
    CreateStatusBar();
    SetStatusText( "Welcome to the app!" );
    
    //Initialize recorder 
    
    m_audio_device_registry.UpdateListOfAudioDevices();
    wxArrayString adList = m_audio_device_registry.GetListofAudioDevices();
   
	//make audio recorder entity
	m_audio_recorder_vec.push_back(new AudioDeviceRecorder());
	
	m_audio_recorder_vec.back()->InitTrack(this);
	
	//set up combo box
	m_audio_device_registry.AddReferenceToComboBox(m_audio_recorder_vec.back()->GetPointerToComboBox());
	
    
	m_audio_device_registry.UpdateAllComboBoxesList();
	
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	
	hbox->Add(m_audio_recorder_vec.at(0)->GetPointerToComboBox());
	hbox->Add(m_audio_recorder_vec.at(0)->GetPointerToRecordButton());
	hbox->Add(m_audio_recorder_vec.at(0)->GetPointerToStopButton());
	
	
	vbox->Add(hbox);
	
	this->SetSizer(vbox);
	this->Layout();
}




void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true ); //close window
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
	//show message box with ok icon, 
	//window title:about hello world
	//message: This is a wxWidgets Helo world sample
    wxMessageBox( "This is an audio recorder helper to be used with binaural audio editor.",
                  "About the Project", wxOK | wxICON_INFORMATION );
}



