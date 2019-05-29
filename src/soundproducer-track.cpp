#include "soundproducer-track.h"

SoundProducerTrack::SoundProducerTrack(const wxString& title) : Track(title)
{
	soundProducerToManipulatePtr = nullptr;
	
	//initialize tracks
	xTrack = new DoubleTrack("X Track");
	yTrack = new DoubleTrack("Y Track");
	zTrack = new DoubleTrack("Z Track");
	
	tempX = 0.0; tempY=0.0; tempZ=0.0;
	
	xTrack->SetReferenceToVarToManipulate(&tempX);
	yTrack->SetReferenceToVarToManipulate(&tempY);
	zTrack->SetReferenceToVarToManipulate(&tempZ);
	
	
	Connect(wxEVT_PAINT, wxPaintEventHandler(Track::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(Track::OnSize));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleTrack::OnLeftMouseClick));
	Connect(wxEVT_CONTEXT_MENU, wxCommandEventHandler(DoubleTrack::OnRightMouseClick));
}


void SoundProducerTrack::FunctionToCallEveryTimeInTimerLoop()
{
	/*
	xTrack->FunctionToCallEveryTimeInTimerLoop();
	yTrack->FunctionToCallEveryTimeInTimerLoop();
	zTrack->FunctionToCallEveryTimeInTimerLoop();
	
	soundProducerToManipulatePtr->SetPositionX(tempX);
	soundProducerToManipulatePtr->SetPositionY(tempY);
	soundProducerToManipulatePtr->SetPositionZ(tempZ);
	*/
}

void SoundProducerTrack::SetReferenceToSoundProducerToManipulate(SoundProducer* thisSoundProducer){soundProducerToManipulatePtr = thisSoundProducer;}

void SoundProducerTrack::InitTrack(wxWindow* parent, std::vector <int> *timeTickVector)
{
	Track::InitTrack(parent,timeTickVector);
	
	xTrack->InitTrack(this,timeTickVector);
	yTrack->InitTrack(this,timeTickVector);
	zTrack->InitTrack(this,timeTickVector);
	
	//arrange tracks
	main_v_box = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal boxes to put tracks in
	wxBoxSizer *hboxTrackX = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *hboxTrackY = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *hboxTrackZ = new wxBoxSizer(wxHORIZONTAL);
	
	int slider_start_x_pos = TRACK_WIDTH / (TIME_TICK_NUM - 1);
	
	//add tracks to their respective boxes
	//keep track at slider start position
	hboxTrackX->Add(xTrack, 0, wxLEFT | wxRIGHT | wxEXPAND, slider_start_x_pos); 
	hboxTrackY->Add(yTrack, 0, wxLEFT | wxRIGHT | wxEXPAND, slider_start_x_pos); 
	hboxTrackZ->Add(zTrack, 0, wxLEFT | wxRIGHT | wxEXPAND, slider_start_x_pos); 
	
	//add horizontal box containing track
	//keep horizontal box containing slider 20 pixels from the top 
	main_v_box->Add(hboxTrackX, 0, wxTOP | wxALIGN_TOP, 50); 
	main_v_box->Add(hboxTrackY, 0, wxTOP | wxALIGN_TOP, 50); 
	main_v_box->Add(hboxTrackZ, 0, wxTOP | wxALIGN_TOP, 50); 
	
	SetSizerAndFit(main_v_box);
}

void SoundProducerTrack::SetupAxisForVariable(double& start, double& end,double& resolution, int& numTick)
{
	xTrack->SetupAxisForVariable(start,end,resolution,numTick);
	yTrack->SetupAxisForVariable(start,end,resolution,numTick);
	zTrack->SetupAxisForVariable(start,end,resolution,numTick);
	
}

void SoundProducerTrack::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	
	xTrack->render(dc);
	yTrack->render(dc);
	zTrack->render(dc);
	
}

void SoundProducerTrack::OnScroll(wxScrollEvent& event)
{
	Refresh();
	
	FitInside();
	
}

void SoundProducerTrack::OnSize(wxSizeEvent& event)
{
	Refresh();
	
	FitInside();
	
}
		
void SoundProducerTrack::SetReferenceToCurrentTimeVariable(double* thisTimeVariable)
{
	Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);
	
	xTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	yTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	zTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
}

void SoundProducerTrack::SetReferenceToTimeTickVector(std::vector <int> *thisVector)
{
	Track::SetReferenceToTimeTickVector(thisVector);
	xTrack->SetReferenceToTimeTickVector(thisVector);
	yTrack->SetReferenceToTimeTickVector(thisVector);
	zTrack->SetReferenceToTimeTickVector(thisVector);
}

std::vector <int> *SoundProducerTrack::GetReferenceToTimeTickVector(){return Track::GetReferenceToTimeTickVector();}

double SoundProducerTrack::GetCurrentTime(){return Track::GetCurrentTime();}
		
void SoundProducerTrack::OnLeftMouseClick(wxMouseEvent& event)
{
	xTrack->logic_left_click();
	yTrack->logic_left_click();
	zTrack->logic_left_click();
	event.Skip();
}

void SoundProducerTrack::OnRightMouseClick(wxCommandEvent& event)
{
	xTrack->logic_right_click();
	yTrack->logic_right_click();
	zTrack->logic_right_click();
	event.Skip();
}
