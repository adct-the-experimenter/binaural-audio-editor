#ifndef _OSGVIEWERWX_H_
#define _OSGVIEWERWX_H_


#include "wx/app.h"
#include "wx/cursor.h"
#include "wx/glcanvas.h" //to work with openscenegraph opengl


#include <osgViewer/Viewer>
#include <string>
#include <wx/filedlg.h> //for file dialog
#include <wx/listctrl.h> //for popup menu

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

#include "soundproducer-track-manager.h"
#include "soundproducer.h"
#include "listener.h"
#include "listener-external.h"

#include "soundproducer-registry.h"

#include "CreateSoundProducerDialog.h"
#include "EditMultipleSoundProducersDialog.h"
#include "HRTF-Test-Dialog.h"
#include "Change-HRTF-Dialog.h"
#include "EditListenerDialog.h"
#include "setup-serial-dialog.h"

#include "openalsoftaudioengine.h"

#include "timeline-frame.h"
#include "soundproducer-track.h"
#include "listener-track.h"

#include "effects-manager.h"

#include "SaveSystem.h"
#include "LoadSystem.h"

#include <iostream>
#include <memory> //for unique_ptr use

#include <sstream> //for string stream

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
	
	void SetReferenceToFunctionToRunKeydown(std::function < void(int&)> thisFunction);
private:
    DECLARE_EVENT_TABLE()

    osg::ref_ptr<osgViewer::GraphicsWindow> _graphics_window;

    wxCursor _oldCursor;
    wxGLContext _context;
    
    std::function < void(int&) > functionToRunKeyDown;
    
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
    void SetListenerExternalReference(ListenerExternal* listenerExt);
    
    void SetEffectsManagerReference(EffectsManager* effectsManager);
    
    SoundProducerTrackManager* GetReferenceToSoundProducerTrackManager();
    
    // Mainframe menu operations
    
    
    void OnOpen(wxCommandEvent& WXUNUSED(event));
    void OnIdle(wxIdleEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	
	void OnListRightClick(wxListEvent &evt);
	void OnPopupClick(wxCommandEvent &evt);
	
	void OnNewProject(wxCommandEvent& WXUNUSED(event));
	void OnSaveProject(wxCommandEvent& WXUNUSED(event));
	void OnLoadProject(wxCommandEvent& WXUNUSED(event));
	
	void OnKeyDown(wxKeyEvent& event); //where camera viewer gets manipulated
	

	//freees all that needs to be freed before load
	void UnloadAll();

	friend class wxOsgApp;
	
private:
    
    enum
	{
		ID_CREATE_SOUND_PRODUCER = 1,
		ID_EDIT_MULTIPLE_SOUND_PRODUCERS,
		ID_PLAY_AUDIO,
		ID_TEST_HRTF,
		ID_CHANGE_HRTF,
		ID_LISTENER_EDIT,
		ID_SETUP_SERIAL,
		ID_CREATE_STANDARD_REVERB_ZONE,
		ID_CREATE_EAX_REVERB_ZONE,
		ID_CREATE_ECHO_ZONE,
		ID_EDIT_MULTIPLE_STANDARD_REVERB_ZONES,
		ID_EDIT_MULTIPLE_EAX_REVERB_ZONES,
		ID_EDIT_MULTIPLE_ECHO_ZONES,
		ID_NEW_PROJECT,
		ID_SAVE_PROJECT,
		ID_LOAD_PROJECT
	};
    
    osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<osg::Group> _rootNode;

	OpenAlSoftAudioEngine* audioEnginePtr;
	
	std::vector < std::unique_ptr <SoundProducer> > *sound_producer_vector_ref;
	
	Listener* listenerPtr;
	ListenerExternal* listenerExternalPtr;
	
	ListenerTrack* m_listener_track;
	
	TimelineFrame *timeFrame;
	
	EffectsManager* effectsManagerPtr;
	
	std::unique_ptr <SoundProducerTrackManager> soundproducertrack_manager_ptr;
	
	std::vector <SoundProducerTrack*> m_soundproducer_track_vec;
	
	wxComboBox* m_sp_toolbar_combobox;
	wxStaticText* sp_toolbar_text;
	
	void OnCreateSoundProducer(wxCommandEvent& event); //function for menu to create and place sound producer
	void CreateSoundProducer(std::string& name, std::string& filePath, ALuint& buffer,double& x, double& y, double& z, bool freeRoam);
	void OnEditMultipleSoundProducers(wxCommandEvent& event); //function for menu to edit current available sound producers
	
	void OnTestHRTF(wxCommandEvent& event); //function for menu to test HRTF and get results
	void OnChangeHRTF(wxCommandEvent& event); //function for menu to instruct how to change HRTF with alsoft-config
    void OnEditListener(wxCommandEvent& event); //function for menu to edit listener position and orientation
    void OnSetupSerial(wxCommandEvent& event); //function for menu to setup serial communication
    
    void OnCreateStandardReverbZone(wxCommandEvent& event); //function for menu to create and place standard reverb zone
    void OnCreateEAXReverbZone(wxCommandEvent& event); //function for menu to create and place EAX reverb zone
    void OnCreateEchoZone(wxCommandEvent& event); //function for menu to create and place echo zone
    
    
    void OnEditMultipleStandardReverbZones(wxCommandEvent& event); //function for menu to edit current available reverb zones
    void OnEditMultipleEAXReverbZones(wxCommandEvent& event); //function for menu to edit current available reverb zones
    void OnEditMultipleEchoZones(wxCommandEvent& event); //function for menu to edit current available echo zones
    
    SoundProducerRegistry soundproducer_registry;
    
    wxBoxSizer* m_add_rm_box_sizer;
    
    void CreateListenerTrack();
    
    void CreateFirstSoundProducerTrack();
    
    wxButton* m_add_soundproducertrack_button;
    void OnAddSoundProducerTrack(wxCommandEvent& event);
   
    void CreateNewSoundProducerTrack();
    int sound_producer_track_count;
    wxButton* m_remove_soundproducertrack_button;
    void OnRemoveSoundProducerTrack(wxCommandEvent& event);
    
    //Save System
    std::unique_ptr <SaveSystem> save_system_ptr;
    
    //Load System
    std::unique_ptr <LoadSystem> load_system_ptr;
    
    void CreateNewProject();
    
    void SaveProject();
    
    void LoadProject();
    
    DECLARE_EVENT_TABLE()
};

class wxOsgApp;

class CheckListenerReverbZoneThread : public wxThread 
{ 
public:     

	CheckListenerReverbZoneThread(EffectsManager* manager,wxOsgApp *handler);
	~CheckListenerReverbZoneThread();
	
protected:
    virtual wxThread::ExitCode Entry();
    wxOsgApp *m_ThreadHandler; 

private:     
	EffectsManager* m_effects_manager_ptr;
};

/* Define a new application type */
//the main of the application
class wxOsgApp : public wxApp
{
public:
	wxOsgApp();
	~wxOsgApp();
	
    bool OnInit();
    
    void KeyDownLogic(int& thisKey);
    
private:
	osg::ref_ptr<osg::Group> rootNode; //geometry node to hold ShapeDrawable objects
	
	std::vector < std::unique_ptr <SoundProducer> > sound_producer_vector; //vector to hold sound producers
	
	OpenAlSoftAudioEngine audio_engine; //class abstraction to handle playing binaural 3D audio
	
	//listener
	std::unique_ptr <Listener> listener;
	std::unique_ptr <ListenerExternal> listenerExternal;
	void initListener();
	
	//effects manager
	std::unique_ptr <EffectsManager> effects_manager_ptr;
	
	CheckListenerReverbZoneThread* m_listener_reverb_thread;
    wxCriticalSection m_ReverbThreadCS;    // protects the  m_listener_reverb_thread pointer
    
    friend class CheckListenerReverbZoneThread;
    
	osg::ref_ptr<osgGA::TrackballManipulator> cameraManipulator; //pointer to camera manipulator
	
	MainFrame* frame;
};

#endif // _WXSIMPLEVIEWERWX_H_
