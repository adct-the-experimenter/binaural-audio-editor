#include "timeline-window.h"

TimelineWindow::TimelineWindow(wxWindow *parent) : wxScrolled<wxWindow>(parent, wxID_ANY)
{	
	m_parent = parent;
	
	current_time_pos = 0;
	
	SetScrollRate( 10, 10 ); //how many pixels to increment when scrolling
    SetVirtualSize( TRACK_WIDTH, TRACK_HEIGHT ); //actual size of what will be scrolled
    SetBackgroundColour( *wxWHITE );

	
	Connect(ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(TimelineWindow::OnScroll));  
	Connect(wxEVT_PAINT, wxPaintEventHandler(TimelineWindow::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(TimelineWindow::OnSize));
	
	TimelineWindow::InitTimeline();
	
	main_v_box = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put names in
	wxBoxSizer *hboxSlider = new wxBoxSizer(wxHORIZONTAL);
	
	//add slider to box
	//keep slider at slider start position
	hboxSlider->Add(m_slider, 0, wxLEFT | wxRIGHT | wxEXPAND, slider_start_x_pos); 
	
	//add horizontal box containing slider
	//keep horizontal box containing slider 20 pixels from the top to keep ruler visible
	main_v_box->Add(hboxSlider, 0, wxTOP | wxALIGN_TOP, 20); 
	
	SetSizerAndFit(main_v_box);
	Center();
}

void TimelineWindow::InitTimeline()
{
	m_slider_value = 0;
	
	//intialize m_time_num
	TimelineWindow::InitTimeVector();
	
	slider_start_x_pos = TRACK_WIDTH / (TIME_TICK_NUM - 1);
	
	m_slider = new wxSlider(this, ID_SLIDER, 0, 0, TRACK_WIDTH, wxPoint(slider_start_x_pos, 30), wxSize(TRACK_WIDTH, -1), wxSL_HORIZONTAL);
	
}

void TimelineWindow::InitTimeVector()
{
	//get linearly spaced vector of doubles
	std::vector <double> thisVector = TimelineWindow::LinearSpacedArray(TIME_START_VALUE,TIME_END_VALUE,TIME_TICK_NUM);
	
	for(size_t i=0; i < thisVector.size(); ++i)
	{
		m_time_num.push_back((int)thisVector[i]);
	}
}

std::vector<double> TimelineWindow::LinearSpacedArray(double a, double b, std::size_t N)
{
	// Linear interpolation following MATLAB linspace
	double h = (b - a) / static_cast<double>(N-1);
	std::vector<double> xs(N);
	std::vector<double>::iterator x;
	double val;
	for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h) {
		*x = val;
	}
	return xs;
}

std::vector <int> * TimelineWindow::GetTimeTickVector(){return &m_time_num;}

void TimelineWindow::OnScroll(wxScrollEvent& event)
{
	m_slider_value = m_slider->GetValue();
	
	current_time_pos = m_slider_value * ((double)TIME_END_VALUE / (double)TRACK_WIDTH);
	
	Refresh(); //for wxDc onPaint stuff 
	
	FitInside(); //for scroll and sizer
}

void TimelineWindow::OnSize(wxSizeEvent& event)
{
	Refresh(); //for wxDc onPaint stuff 
	
	FitInside(); //for resize and sizer 
}

void TimelineWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	DoPrepareDC(dc); //prepare device context for drawing a scrolling image
	
	//Initialize variables for drawing vertical timeline line indicating current position
	wxPen pen(wxColour(0, 0, 0)); //make pen color very black
	dc.SetPen(pen);
	
	wxBrush brush1(wxColour(197, 108, 0));
	dc.SetBrush(brush1);
	
	
	//x coordinate of vertical line representing current position in time
	wxCoord x = m_slider_value + slider_start_x_pos;
	
	dc.DrawRectangle( wxRect(x, 0, 2, VERTICAL_LINE_HEIGHT_TIME) );
	
	
	//initialize variables for timeline ruler
	
	wxFont font = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
	dc.SetFont(font);
	
	int step = (int) round( TRACK_WIDTH / (TIME_TICK_NUM-1) );

	dc.SetPen(wxPen(wxColour(90, 80, 60)));
	
	for ( int i=1; i <= (int)m_time_num.size(); i++ ) 
	{
		dc.DrawLine(i*step, 1, i*step, 10);
		dc.DrawText( wxString::Format( wxT("%ds"), m_time_num[i-1] ) , i*step, 10);
	}
}

void TimelineWindow::SetCurrentTimePosition(double& thisTime)
{
	Refresh(); //for updating paint drawing everytime current time position set
	
	current_time_pos = thisTime;
	
	//set slider to current time position
	int graphical_value = thisTime * (TRACK_WIDTH / TIME_END_VALUE);
	m_slider->SetValue(graphical_value);
	m_slider_value = graphical_value;
}

double TimelineWindow::GetCurrentTimePosition(){return current_time_pos;}

void TimelineWindow::AddTrack(Track* thisTrack, int& space)
{
	//initialize track
	thisTrack->InitTrack(this,&m_time_num);
	thisTrack->SetReferenceToCurrentTimeVariable(&current_time_pos);
	
	//make horizontal box to put track in
	wxBoxSizer *hboxTrack = new wxBoxSizer(wxHORIZONTAL);
	
	//put track in horizontal box
	hboxTrack->Add(thisTrack, 0, wxRIGHT | wxLEFT, slider_start_x_pos);
	
	//add track to timeline window
	main_v_box->Add(hboxTrack, 1, wxTOP | wxALIGN_TOP,space);
	
	SetSizerAndFit(main_v_box);
}

void TimelineWindow::AddSpacerBlock(int space)
{
	main_v_box->AddSpacer(space);
	SetSizerAndFit(main_v_box);
}

void TimelineWindow::AddBoxSizer(wxSizer *sizer,int proportion,int flag,int border,wxObject *userData)
{
	//	flags = OR-combination of flags affecting sizer's behaviour. See wxSizer flags list for details. 
	main_v_box->Add(sizer,proportion,flag,border,userData);
	
	SetSizerAndFit(main_v_box);
}

void TimelineWindow::AddText(wxString thisText, wxPoint thisPoint)
{
	wxStaticText *st1 = new wxStaticText(this, wxID_ANY, thisText,thisPoint );
}

wxSlider* TimelineWindow::getSliderReference(){return m_slider;}
double* TimelineWindow::getPointerToCurrentTimeReference(){return &current_time_pos;}
