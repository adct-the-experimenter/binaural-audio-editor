#include "editor-graph.h"

 
EditorGraph::EditorGraph(wxWindow* parent) : wxPanel(parent)
{
	
}

void EditorGraph::SetReferenceToTimeTickVector(std::vector <int> *thisVector){timeTickVectorPtr = thisVector;}

int EditorGraph::GetVerticalGraphValueAtThisTime(double& thisTime, bool& legitValue)
{
	//check if there is already a point at that time value
	if ( map_time.find(thisTime) == map_time.end() ) 
	{
		//if not found, do nothing
		legitValue = false;
		return 0;
	}
	else
	{
		//if found
		
		//get iterator to vector from time map
		std::unordered_map<double,wxPoint>::const_iterator got = map_time.find (thisTime);
		wxPoint point = got->second;
		
		legitValue = true; 
		return (int)(point).y;
	}
}

template <typename T>
void EditorGraph::render(wxDC&  dc,std::vector <T> *verticalAxisVector)
{
	DrawHorizontalAxis(dc);
	DrawVerticalAxis(dc,verticalAxisVector);
	DrawCurrentPointsOnGraph(dc);
}

template void EditorGraph::render<double>( wxDC& , std::vector<double>*);

void EditorGraph::DrawHorizontalAxis(wxDC& dc)
{
	wxFont font = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
	dc.SetFont(font);
	
	int step = (int) round( TRACK_WIDTH / (TIME_TICK_NUM-1) );
	int offset = TRACK_WIDTH / (TIME_TICK_NUM - 1);
	
	dc.SetPen(wxPen(wxColour(90, 80, 60)));
	
	for ( int i=1; i <= (int)timeTickVectorPtr->size(); i++ ) 
	{
		dc.DrawLine(i*step - offset, 1, i*step - offset , 10);
		
		//skip drawing the zero tick maker because it is mixes with text of vertical axis
		if(i != 1)
		{
			dc.DrawText( wxString::Format( wxT("%ds"), (int)timeTickVectorPtr->at(i-1) ) , i*step - offset, 10);
		}
		
	}
}

template <typename T>
void EditorGraph::DrawVerticalAxis(wxDC& dc,std::vector <T> *verticalAxisVector)
{
	wxFont font = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
	dc.SetFont(font);
	
	int step = (int) round( TRACK_HEIGHT / (verticalAxisVector->size()) );

	dc.SetPen(wxPen(wxColour(90, 80, 60)));
	
	for ( size_t i=1; i <= verticalAxisVector->size(); i++ ) 
	{
		dc.DrawLine(1, i*step, 10, i*step);
		//start at end to draw positive numbers on top
		dc.DrawText( wxString::Format( wxT("%d"), (int)verticalAxisVector->at(verticalAxisVector->size() - i) ) , 0, (i*step) - 10);
	}
}

template void EditorGraph::DrawVerticalAxis<double>( wxDC& , std::vector<double>*);

template <typename T>
void EditorGraph::mouseDownLeftClick(T& vertStart, T& vertEnd, T& vertRes,
							double& time, int& relMouseY, bool& legitValues)
{
	EditorGraph::PlacePointByMouse(vertStart,vertEnd,vertRes,time,relMouseY,legitValues);
}

template void EditorGraph::mouseDownLeftClick<double>( double& vertStart, double& vertEnd, double& vertRes,
														double& time, int& relMouseY, bool& legitValues);

void EditorGraph::mouseDownRightClick(double& time,bool& legitValue)
{
	EditorGraph::RemovePointByMouse(time,legitValue);
}

template <typename T>
void EditorGraph::PlacePointByMouse(T& vertStart, T& vertEnd, T& vertRes, 
									double& time, int& relMouseY, bool& legitValues)
{
	//get graphical coordinates of where mouse left clicked relative to track panel
	int mouseX = wxGetMousePosition().x - this->GetScreenPosition().x; 
	int mouseY = wxGetMousePosition().y - this->GetScreenPosition().y;
	
	//convert mouse x to time value
	double thisTime = mouseX * ((double)TIME_END_VALUE / (double)TRACK_WIDTH);
	
	//make time value a multiple of timer resolution
	thisTime = round (thisTime / (double(TIME_RESOLUTION) / 1000)) * (double(TIME_RESOLUTION) / 1000);
	//std::cout << "add time:" << thisTime << std::endl;
	
	legitValues = false;
	//check if there is already a point at that time value
	if ( map_time.find(thisTime) == map_time.end() ) 
	{
		//make mouseY a multiple of vertical resolution in pixels
		//convert to value, divide by resolution, round result, multiply by resolution, convert back to pixel value
		mouseY = round(mouseY * ( (vertEnd - vertStart) / double(TRACK_HEIGHT) ) * ( 1 / vertRes) ) * vertRes * ( double(TRACK_HEIGHT) / (vertEnd - vertStart) );

		//std::cout << "mouseY is " << mouseY << "at time " << thisTime << " in place point by mouse.";
		//if not found
		 
		//put into vector of graph points
		//graph_points.push_back( wxPoint(mouseX,mouseY) );
		
		//put into time map
		//the iterator to element pushed back is actually the one before end iterator due to vectors having an extra element
		//to carry out data operations!
		//std::vector<wxPoint>::iterator it = graph_points.end()-1; 
		map_time.emplace(thisTime, wxPoint(mouseX,mouseY));
		
		//save time and mouse y to input variables
		time = thisTime;
		relMouseY = mouseY;
		legitValues = true;
	} 
	
}

template void EditorGraph::PlacePointByMouse<double>(double& vertStart, double& vertEnd, double& vertRes,
													 double& time, int& relMouseY, bool& legitValues);

void EditorGraph::RemovePointByMouse(double& time,bool& legitValue)
{
	//get graphical coordinates of where mouse left clicked relative to track panel
	int mouseX = wxGetMousePosition().x - this->GetScreenPosition().x; 
	
	//convert mouse x to time value
	double thisTime = mouseX * ((double)TIME_END_VALUE / (double)TRACK_WIDTH);
	//make time value a multiple of timer resolution
	thisTime = round (thisTime / (double(TIME_RESOLUTION) / 1000)) * (double(TIME_RESOLUTION) / 1000);
	
	//std::cout << "remove time:" << thisTime << std::endl;
	
	//check if there is already a point at that time value
	if ( map_time.find(thisTime) == map_time.end() ) 
	{
		//if not found, do nothing
		legitValue = false;
	}
	else
	{
		//if found, remove the point 
		
		//save time point info
		legitValue = true;
		time = thisTime;
		
		//get iterator to vector from time map
		//std::unordered_map<double,wxPoint>::const_iterator got = map_time.find (thisTime);
		//wxPoint point = got->second;
		
		//remove point from vector of graph points
		//graph_points.erase(it);
		//remove from time map
		map_time.erase(thisTime);
	}
}

void EditorGraph::DrawCurrentPointsOnGraph(wxDC& dc)
{
	// draw a circle
    dc.SetBrush(*wxBLACK_BRUSH);
    
    
	for ( auto it = map_time.begin(); it != map_time.end(); ++it )
	{
		dc.DrawCircle( it->second, 2 );
	}
    
}

template <typename T>
void EditorGraph::PlacePointsFromThisMap(DDMap& thisMap,T& vertStart, T& vertEnd, T& vertRes)
{
	for (DDMap::iterator it = thisMap.begin(); it!= thisMap.end(); ++it)
	{
		
		double thisTime = it->first; 
		thisTime = round (thisTime / (double(TIME_RESOLUTION) / 1000)) * (double(TIME_RESOLUTION) / 1000);
		
		double xPoint = thisTime * ( (double)TRACK_WIDTH / (double)TIME_END_VALUE );
		//std::cout << "x point:" << xPoint << std::endl;
		
		double out = it->second;
		double yPoint = round( (vertEnd - out) * ( double(TRACK_HEIGHT) / (vertEnd - vertStart) ) *  vertRes ) * (1 / vertRes) ;
		//std::cout << "y point:" << yPoint << std::endl;
		
		map_time.emplace(thisTime, wxPoint(xPoint,yPoint));
		
	}
}

template void EditorGraph::PlacePointsFromThisMap<double>( DDMap& thisMap,double& vertStart, double& vertEnd, double& vertRes);
