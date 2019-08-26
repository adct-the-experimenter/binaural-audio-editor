#ifndef TRACK_H
#define TRACK_H

#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/panel.h>

#include <iostream>
#include <vector>

#include "../parameters.h"

class Track : public wxPanel
{

public:
    Track(const wxString& title);
    
    virtual void InitTrack(wxWindow* parent, std::vector <int> *timeTickVector);

    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnScroll(wxScrollEvent& event);
	virtual void OnSize(wxSizeEvent& event);
    
    void SetReferenceToCurrentTimeVariable(double* thisTimeVariable);
    void SetReferenceToTimeTickVector(std::vector <int> *thisVector);
	
	void SetTitle(wxString thisTitle);
    wxString GetTitle();
    
    std::vector <int> *GetReferenceToTimeTickVector();
    
    double GetCurrentTime();
    
    //function to call in timer loop, variable to manipulate gets changed here
    virtual void FunctionToCallInPlayState() = 0;
    virtual void FunctionToCallInPauseState() = 0;
    virtual void FunctionToCallInRewindState() = 0;
    virtual void FunctionToCallInFastForwardState() = 0;
    virtual void FunctionToCallInNullState() = 0;
    
private:
	
	wxWindow* m_parent;
	
	wxString m_title; //title of the track
	
    double* current_time_pos_pointer;
    std::vector <int> *ptrToTimeTickVector;
    
};

#endif
