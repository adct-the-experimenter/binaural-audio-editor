#include "double-track.h"

DoubleTrack::DoubleTrack(const wxString& title)  : Track (title)
{
	varToManipulatePtr = nullptr;
	graphEditor = nullptr;
	
	playbackControlsPtr = nullptr;
	
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleTrack::OnLeftMouseClick));
	Connect(wxEVT_CONTEXT_MENU, wxCommandEventHandler(DoubleTrack::OnRightMouseClick));
		
}

void DoubleTrack::FunctionToCallInPlayState()
{
	if(varToManipulatePtr != nullptr)
	{
		//change variable
		if(graphEditor != nullptr)
		{
			double thisTime = DoubleTrack::GetCurrentTime();
			
			
			//check if there is  a point at that time value
			if ( map_time_output.find(thisTime) == map_time_output.end() ) 
			{
				//if not found, do nothing
			}
			else
			{
				//if found

				//get iterator to vector from time map
				std::unordered_map<double,double>::const_iterator got = map_time_output.find (thisTime);
				
				//std::cout << "value " << got->second << " found at time " << got->first << std::endl;
				if(*varToManipulatePtr != got->second)
				{
					*varToManipulatePtr = got->second;
					
					//call this void function after variable change
					//if it has a callable function target
					if(func_after_var_change)
					{
						func_after_var_change();
					}
				}
			}
		}
	}
}

void DoubleTrack::FunctionToCallInPauseState(){}

void DoubleTrack::FunctionToCallInRewindState(){}

void DoubleTrack::FunctionToCallInFastForwardState(){}

void DoubleTrack::FunctionToCallInNullState(){}

void DoubleTrack::SetReferenceToCurrentTimeVariable(double* thisTimeVariable){Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);}

std::vector <int> * DoubleTrack::GetReferenceToTimeTickVector(){return Track::GetReferenceToTimeTickVector();}

void DoubleTrack::SetReferenceToTimeTickVector(std::vector <int> *thisVector){Track::SetReferenceToTimeTickVector(thisVector);}

void DoubleTrack::SetReferenceToVarToManipulate(double* thisVar){varToManipulatePtr = thisVar;}

double DoubleTrack::GetCurrentTime(){return Track::GetCurrentTime();}

void DoubleTrack::SetTitle(wxString thisTitle){Track::SetTitle(thisTitle);}
wxString DoubleTrack::GetTitle(){return Track::GetTitle();}

void DoubleTrack::InitTrack(wxWindow* parent, std::vector <int> *timeTickVector)
{
	Track::InitTrack(parent,timeTickVector);
	
	//initialize graph editor
	graphEditor = new EditorGraph(this);
	graphEditor->SetReferenceToTimeTickVector(timeTickVector);
	
	//wxStaticText *st1 = new wxStaticText(parent, wxID_ANY, DoubleTrack::GetTitle(), 
	//									wxPoint(this->GetScreenPosition().x,this->GetScreenPosition().y) );
}

void DoubleTrack::SetupAxisForVariable(double& start, double& end,double& resolution, int& numTick)
{
	verticalStart = start;
	verticalEnd = end;
	verticalNumTicks = numTick;
	verticalResolution = resolution;
	
	//setup tick marks
	DoubleTrack::InitVerticalAxis();
}

void DoubleTrack::InitVerticalAxis()
{
	//get linearly spaced vector of doubles
	std::vector <double> thisVector = DoubleTrack::LinearSpacedArray(verticalStart,verticalEnd,(size_t)verticalNumTicks);
	
	for(size_t i=0; i < thisVector.size(); ++i)
	{
		m_vertical_var_num.push_back(thisVector[i]);
	}
}

std::vector<double> DoubleTrack::LinearSpacedArray(double a, double b, std::size_t N)
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

void DoubleTrack::OnSize(wxSizeEvent& event)
{
	Refresh();

	FitInside();
}

void DoubleTrack::OnScroll(wxScrollEvent& event)
{
	//std::cout << "Scroll called! \n";
	Refresh(); //for wxDc onPaint stuff 
	
	FitInside(); //for scroll and sizer
}


void DoubleTrack::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	DoubleTrack::render(dc);
	
	event.Skip();
}

void DoubleTrack::render(wxDC& dc)
{
	PrepareDC(dc); //prepare device context for drawing a scrolling image
	
	graphEditor->render(dc,&m_vertical_var_num);
}

void DoubleTrack::OnLeftMouseClick(wxMouseEvent& event)
{
	DoubleTrack::logic_left_click();
	event.Skip();
}

void DoubleTrack::OnRightMouseClick(wxCommandEvent& event)
{
	DoubleTrack::logic_right_click();
	event.Skip();
}

void DoubleTrack::logic_left_click()
{
	double mouseTimePoint; 
	int mouseYPoint;
	bool legitValues = true;
	
	graphEditor->mouseDownLeftClick(verticalStart,verticalEnd,verticalResolution,
									mouseTimePoint, mouseYPoint,legitValues);
	
	if(legitValues)
	{
		//convert mouse y point to output value
		double output = ((double)TRACK_HEIGHT - mouseYPoint) * ((verticalEnd - verticalStart) / double(TRACK_HEIGHT) ) - verticalEnd;
		
		//put it in the map
		map_time_output.emplace(mouseTimePoint, output);
		
		Refresh();
	}
	
	
	
}

void DoubleTrack::logic_right_click()
{
	double mouseTimePoint;
	bool legitValue = true;
	
	graphEditor->mouseDownRightClick(mouseTimePoint, legitValue);
	
	if(legitValue)
	{
		//remove point from the map
		map_time_output.erase(mouseTimePoint);
		
		Refresh();
	}
	
	
}

void DoubleTrack::SetFunctionToCallAfterVariableChange(std::function < void() > thisFunction){func_after_var_change = thisFunction;}

void DoubleTrack::SetReferenceToPlaybackControls(PlaybackControls* controls){playbackControlsPtr = controls;}
PlaybackControls* DoubleTrack::GetReferenceToPlaybackControls(){return playbackControlsPtr;}

DDMap* DoubleTrack::GetPointerToTimeValueMap(){return &map_time_output;}

void DoubleTrack::LoadDataFromThisTimeValueMap(DDMap& map)
{
	map_time_output = map;
	
	graphEditor->PlacePointsFromThisMap(map_time_output,verticalStart,verticalEnd,verticalResolution);
}
