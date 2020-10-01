// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cassert>
#include <cmath>

#include "time.h"

#include "sndfile.h"

#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

#include "audio-device-recorder.h"
#include "audio-device-registry.h"


struct AudioEntity {
	AudioDeviceRecorder recorder;
	wxComboBox* ptrToComboBox;
	wxButton* ptrToRecordButton;
};

//override wxApp to initialize program 
class MyApp: public wxApp
{
public:
    virtual bool OnInit(); //initialize program
    
    void OnIdle(wxIdleEvent &event);
};

//override wxFrame to make new custom window
class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    
    void OnExit(wxCommandEvent& event);
    
    void OnAbout(wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE();
    
    AudioDeviceRegistry m_audio_device_registry;
    
    std::vector <AudioEntity> m_audio_entity_vec;
    
};

//unique identifier to use to react to menu command
enum
{
    ID_Hello = 1
};

