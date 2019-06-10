#ifndef LISTENER_TRACK_H
#define LISTENER_TRACK_H


#include <wx/wx.h>

#include "listener.h"
#include "double-track.h"

#include <memory>
#include <wx/combobox.h>
#include <wx/arrstr.h>

//class to manipulate x,y z position of sound producer
class ListenerTrack : public Track
{
public:
	ListenerTrack(const wxString& title);
	
	
	DoubleTrack* GetReferenceToXTrack();
	DoubleTrack* GetReferenceToYTrack();
	DoubleTrack* GetReferenceToZTrack();
	
	//Listener Editing
	void SetReferenceToListenerToManipulate(Listener* thisListener);
	
	//Double Track related functions
	void SetupAxisForVariable(double& start, double& end, double& resolution, int& numTick);
	
	void OnLeftMouseClick(wxMouseEvent& event);
	void OnRightMouseClick(wxCommandEvent& event);
	
	//Functions inherited from Track
	virtual void InitTrack(wxWindow* parent, std::vector <int> *timeTickVector);

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnScroll(wxScrollEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	
	void SetReferenceToCurrentTimeVariable(double* thisTimeVariable);
	void SetReferenceToTimeTickVector(std::vector <int> *thisVector);
	
	std::vector <int> *GetReferenceToTimeTickVector();
	
	double GetCurrentTime();
	
	//function to call in timer loop, variable to manipulate gets changed here
	virtual void FunctionToCallInPlayState();
    virtual void FunctionToCallInPauseState();
    virtual void FunctionToCallInRewindState();
    virtual void FunctionToCallInFastForwardState();
    virtual void FunctionToCallInNullState();
	
private:
	Listener* listenerToManipulatePtr;
	
	DoubleTrack* xTrack;
	DoubleTrack* yTrack;
	DoubleTrack* zTrack;
	
	//variables to hold temporary values for sound producer position
	double tempX,tempY,tempZ;
};

#endif
