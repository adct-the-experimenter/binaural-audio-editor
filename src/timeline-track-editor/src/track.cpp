#include "track.h"

Track::Track(const wxString& title)  : wxPanel()
{
	m_title = title;
	
	Connect(wxEVT_PAINT, wxPaintEventHandler(Track::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(Track::OnSize));
	
	current_time_pos_pointer = nullptr;
	ptrToTimeTickVector = nullptr;
}


void Track::SetReferenceToCurrentTimeVariable(double* thisTimeVariable){current_time_pos_pointer = thisTimeVariable;}
void Track::SetReferenceToTimeTickVector(std::vector <int> *thisVector){ptrToTimeTickVector = thisVector;}

std::vector <int> *Track::GetReferenceToTimeTickVector(){return ptrToTimeTickVector;}

void Track::SetTitle(wxString thisTitle){m_title = thisTitle;}
wxString Track::GetTitle(){return m_title;}

void Track::InitTrack(wxWindow* parent, std::vector <int> *timeTickVector)
{
	m_parent = parent;
	
	ptrToTimeTickVector = timeTickVector;
	this->Create(parent, wxID_ANY, wxPoint(wxDefaultPosition.x,wxDefaultPosition.y + 50), wxSize(TRACK_WIDTH, TRACK_HEIGHT),wxTAB_TRAVERSAL,m_title);
	this->SetBackgroundColour( *wxLIGHT_GREY );
	this->Show();
}

void Track::OnSize(wxSizeEvent& event)
{
	Refresh();

	FitInside();
	
	event.Skip();
}

void Track::OnScroll(wxScrollEvent& event)
{
	//std::cout << "Scroll called! \n";
	Refresh(); //for wxDc onPaint stuff 
	
	FitInside(); //for scroll and sizer
	
	event.Skip();
}


void Track::OnPaint(wxPaintEvent& event)
{
	//std::cout << "Current Time in Track:" << *current_time_pos_pointer << std::endl;
	event.Skip();
}

double Track::GetCurrentTime(){return *current_time_pos_pointer;}

