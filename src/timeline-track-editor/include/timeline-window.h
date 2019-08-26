#ifndef TIMELINE_WINDOW_H
#define TIMELINE_WINDOW_H

#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/button.h>

#include <iostream>
#include <vector>

#include "track.h"

#include "../parameters.h"

class TimelineWindow : public wxScrolled<wxWindow>
{
public:
    TimelineWindow(wxWindow *parent);
	
    void OnPaint(wxPaintEvent& event);
    void OnScroll(wxScrollEvent& event);
	void OnSize(wxSizeEvent& event);
    
	//Timeline related functions
    
    void SetCurrentTimePosition(double& thisTime);
    double GetCurrentTimePosition();
    
    //Track related functions
    
    void AddTrack(Track* thisTrack, int& space);
    wxSlider* getSliderReference(); //for updating track time
    double* getPointerToCurrentTimeReference();
    
    std::vector <int> *GetTimeTickVector(); //use to draw time ticks in tracks
    
    //Display related functions
    
    void AddSpacerBlock(int space); //function to add space between boxes containing Tracks or panels
    void AddText(wxString thisText, wxPoint thisPoint);//function to add text anywhere in the window
    void AddBoxSizer(wxSizer *sizer, int proportion=0, int flag=0, int border=0, wxObject *userData=nullptr);
    
private:

	wxWindow* m_parent;
	
	//main box that contains all elements and places them vertically
	wxBoxSizer* main_v_box;  
	
	wxSlider *m_slider;
    int m_slider_value;
    
	std::vector <int> m_time_num; // numbers to display on ruler
	int slider_start_x_pos; //pixel x position of slider set based on ticks
	
	void InitTimeline(); //function to initialize parameters for timeline
	
	void InitTimeVector(); //function to initialze m_time_num
	std::vector<double> LinearSpacedArray(double a, double b, std::size_t N);
	
	double current_time_pos; //the current time that the vertical line shows
};


const int ID_SLIDER = 100;

#endif
