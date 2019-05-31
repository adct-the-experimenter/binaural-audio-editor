#ifndef TIMELINE_WINDOW_H
#define TIMELINE_WINDOW_H

#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/button.h>

#include <iostream>
#include <vector>

#include "track.h"

#include "parameters.h"

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
