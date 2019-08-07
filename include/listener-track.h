#ifndef LISTENER_TRACK_H
#define LISTENER_TRACK_H


#include <wx/wx.h>

#include "listener.h"
#include "double-track.h"

#include <memory>
#include <wx/combobox.h>
#include <wx/arrstr.h>

#include <boost/math/quaternion.hpp> //for using quaternion to change orientation
#include "SimpleSerial.h" //for getting serial data
#include <boost/algorithm/string.hpp> //for splitting string

//class to manipulate x,y z position of sound producer
class ListenerTrack : public Track
{
public:
	ListenerTrack(const wxString& title);
	
	
	DoubleTrack* GetReferenceToXTrack();
	DoubleTrack* GetReferenceToYTrack();
	DoubleTrack* GetReferenceToZTrack();
	
	DoubleTrack* GetReferenceToQuatWTrack();
	DoubleTrack* GetReferenceToQuatXTrack();
	DoubleTrack* GetReferenceToQuatYTrack();
	DoubleTrack* GetReferenceToQuatZTrack();
	
	//Listener Editing
	void SetReferenceToListenerToManipulate(Listener* thisListener);
	
	//Double Track related functions
	void SetupAxisForPositionVariable(double& start, double& end, double& resolution, int& numTick);
	void SetupAxisForOrientationVariable(double& start, double& end, double& resolution, int& numTick);
	
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
	
	DoubleTrack* wQuatTrack;
	DoubleTrack* xQuatTrack;
	DoubleTrack* yQuatTrack;
	DoubleTrack* zQuatTrack;
	
	
	//variables to hold temporary values for listener position and orientation
	double tempX,tempY,tempZ,tempQuatX,tempQuatY,tempQuatZ,tempQuatW;
	
	//quaternions for intial forward vector and up vector directions
	boost::math::quaternion <float> forward_vector_quaternion;
	boost::math::quaternion <float> up_vector_quaternion;
};

#endif
