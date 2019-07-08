#ifndef _OSGVIEWERWX_H_
#define _OSGVIEWERWX_H_

#include "wx/defs.h"
#include "wx/app.h"
#include "wx/cursor.h"
#include "wx/glcanvas.h" //to work with openscenegraph opengl


#include <osgViewer/Viewer>
#include <string>
#include <wx/filedlg.h> //for file dialog
#include <wx/listctrl.h> //for popup menu



// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// For wxCURSOR_BLANK below, but isn't used a.t.m.
//#ifdef WIN32
//#include "wx/msw/wx.rc"
//#endif

#include <wx/image.h>
#include <wx/menu.h>

#include <wx/thread.h> //for wxIsMainThread function

#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/Group>

#include "openalsoftaudioengine.h"

#include <osgGA/OrbitManipulator>
#include <osgGA/TrackballManipulator>


#include "soundproducer.h"
#include "listener.h"

#include "soundproducer-registry.h"

#include "CreateSoundProducerDialog.h"
#include "EditMultipleSoundProducersDialog.h"
#include "HRTF-Test-Dialog.h"
#include "EditListenerDialog.h"

#include "openalsoftaudioengine.h"

#include "timeline-frame.h"
#include "soundproducer-track.h"
#include "listener-track.h"

#include <iostream>
#include <memory> //for unique_ptr use

#include <sstream> //for string stream
#include <list>

class GraphicsWindowWX;

class OSGCanvas : public wxGLCanvas
{
public:
    OSGCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("Binaural Audio Editor Canvas"),
        int *attributes = 0);

    virtual ~OSGCanvas();

    void SetGraphicsWindow(osgViewer::GraphicsWindow *gw)   { _graphics_window = gw; }

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);

	//where camera viewer gets manipulated by mouse movements
	
    void OnChar(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    
    void OnKeyDown(wxKeyEvent& event); 
	
    void OnMouseEnter(wxMouseEvent &event);
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnMouseMotion(wxMouseEvent &event);
    void OnMouseWheel(wxMouseEvent &event);

    void UseCursor(bool value);

	void SetContextCurrent();
	
	
private:
    DECLARE_EVENT_TABLE()

    osg::ref_ptr<osgViewer::GraphicsWindow> _graphics_window;

    wxCursor _oldCursor;
    wxGLContext _context;
    
};

class GraphicsWindowWX : public osgViewer::GraphicsWindow
{
public:
    GraphicsWindowWX(OSGCanvas *canvas);
    ~GraphicsWindowWX();

    void init();

    //
    // GraphicsWindow interface
    //
    void grabFocus();
    void grabFocusIfPointerInWindow();
    void useCursor(bool cursorOn);

    bool makeCurrentImplementation();
    void swapBuffersImplementation();

    // not implemented yet...just use dummy implementation to get working.
    virtual bool valid() const { return true; }
    virtual bool realizeImplementation() { return true; }
    virtual bool isRealizedImplementation() const  { return _canvas->IsShownOnScreen(); }
    virtual void closeImplementation() {}
    virtual bool releaseContextImplementation() { return true; }

private:
    // XXX need to set _canvas to NULL when the canvas is deleted by
    // its parent. for this, need to add event handler in OSGCanvas
    OSGCanvas*  _canvas;
};


class MainFrame : public wxFrame
{
public:
    MainFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
        const wxSize& size, OpenAlSoftAudioEngine* thisAudioEngine, long style = wxDEFAULT_FRAME_STYLE);
	
	//for connecting mainframe to wxOsgApp
	
    void SetViewer(osgViewer::Viewer *viewer);
    void SetRootNode(osg::Group* root); 
    void SetSoundProducerVectorRef(std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector);
    void SetAudioEngineReference(OpenAlSoftAudioEngine* audioEngine);
    void SetListenerReference(Listener* listener);
    
    // Mainframe menu operations
    
    void OnOpen(wxCommandEvent& WXUNUSED(event));
    void OnIdle(wxIdleEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnPlayAudio(wxCommandEvent& event);
	
	void OnListRightClick(wxListEvent &evt);
	void OnPopupClick(wxCommandEvent &evt);
	
	
	
	void OnKeyDown(wxKeyEvent& event); //where camera viewer gets manipulated
	
private:
    
    enum
	{
		ID_CREATE_SOUND_PRODUCER = 1,
		ID_EDIT_MULTIPLE_SOUND_PRODUCERS,
		ID_PLAY_AUDIO,
		ID_TEST_HRTF,
		ID_CHANGE_HRTF,
		ID_LISTENER_EDIT
	};
    
    osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<osg::Group> _rootNode;

	OpenAlSoftAudioEngine* audioEnginePtr;
	
	std::list < std::unique_ptr <SoundProducer> > *sound_producer_list_ref;
	
	Listener* listenerPtr;
	ListenerTrack* m_listener_track;
	
	TimelineFrame *timeFrame;
	
	std::vector <SoundProducerTrack*> m_soundproducer_track_vec;
	
	void OnCreateSoundProducer(wxCommandEvent& event); //function for menu to create and place sound producer
	void CreateSoundProducer(std::string& name, std::string& filePath, ALuint& buffer,double& x, double& y, double& z);
	
	void OnEditMultipleSoundProducers(wxCommandEvent& event); //function for menu to edit current available sound producers
	void OnTestHRTF(wxCommandEvent& event); //function for menu to test HRTF and get results
	void OnChangeHRTF(wxCommandEvent& event); //function for menu to instruct how to change HRTF with alsoft-config
    void OnEditListener(wxCommandEvent& event); //function for menu to edit listener position and orientation
    
    SoundProducerRegistry soundproducer_registry;
    
    wxBoxSizer* m_add_rm_box_sizer;
    
    wxButton* m_add_soundproducertrack_button;
    void OnAddSoundProducerTrack(wxCommandEvent& event);
   
    void CreateNewSoundProducerTrack();
    int sound_producer_track_count;
    wxButton* m_remove_soundproducertrack_button;
    void OnRemoveSoundProducerTrack(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE()
};

/* Define a new application type */
//the main of the application
class wxOsgApp : public wxApp
{
public:
    bool OnInit();
    
    
private:
	osg::ref_ptr<osg::Group> rootNode; //geometry node to hold ShapeDrawable objects
	
	std::list < std::unique_ptr <SoundProducer> > sound_producer_list; //vector to hold sound producers
	
	OpenAlSoftAudioEngine audio_engine; //class abstraction to handle playing binaural 3D audio
	
	std::unique_ptr <Listener> listener;
	void initListener();
	
	osg::ref_ptr<osgGA::TrackballManipulator> cameraManipulator; //pointer to camera manipulator
};

#endif // _WXSIMPLEVIEWERWX_H_
