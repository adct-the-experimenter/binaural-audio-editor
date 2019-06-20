#include "audio-graph.h"

AudioGraph::AudioGraph(wxWindow* parent) : wxPanel(parent)
{
	
}
    
void AudioGraph::mouseDownLeftClick()
{
	
}

void AudioGraph::mouseDownRightClick()
{
	
}

void AudioGraph::render(wxDC& dc, std::vector <double> *verticalAxisVector)
{
	AudioGraph::DrawVerticalAxis(dc,verticalAxisVector);
	AudioGraph::DrawHorizontalAxis(dc);
	AudioGraph::DrawCurrentDataOnGraph(dc);
}

void AudioGraph::SetReferenceToTimeTickVector(std::vector <int> *thisVector){timeTickVectorPtr = thisVector;}


int AudioGraph::GetVerticalGraphValueAtThisTime(double& thisTime,bool& legitValue)
{
	
}

void AudioGraph::PlotAudioDataToGraph(std::vector <double> *audio_data, int sample_rate,
										double& verticalStart, double& verticalEnd, double& verticalResolution)
{
	double time_resolution = 0.1;
	
	//calculate number of samples in time_resolution seconds
	int num_samples = time_resolution * sample_rate;
	
	//keep track of every time 0.1 seconds of audio passed
	size_t count = 0;
	
	double min = 0.0f;
	double max = 0.0f;
	
	size_t min_time_count = 0;
	size_t max_time_count = 0;
	
	//for every sample
	for(size_t i=0; i < audio_data->size(); i++)
	{
		//std::cout << "i:" << i << " value:" << audio_data->at(i);
		
		//at end of time_resolution seconds
		if(count == num_samples)
		{	
			//push points for min and max into graph
			
			int xMinTime = (min_time_count / sample_rate) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMinGain = min * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			graph_points.push_back(wxPoint(xMinTime,yMinGain));
			
			int xMaxTime = (max_time_count / sample_rate) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMaxGain = max * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			graph_points.push_back(wxPoint(xMaxTime,yMaxGain));
			
			
			
			//reset min and max
			min = 0;
			max = 0;
			
			count = 0; //reset count
			
		}
		
		if(audio_data->at(i) < min && audio_data->at(i) >= -1)
		{
			min = audio_data->at(i); 
			min_time_count = i;
		}
		
		if(audio_data->at(i) > max && audio_data->at(i) <= 1)
		{
			max = audio_data->at(i); 
			max_time_count = i;
		}
		
		//increment count
		count++;
	}
}

void AudioGraph::DrawCurrentDataOnGraph(wxDC& dc)
{
	// draw a circle
    dc.SetBrush(*wxBLACK_BRUSH);
    for(size_t i=0; i < graph_points.size(); i++)
    {
		dc.DrawCircle( graph_points.at(i), 3 );
	}
}

void AudioGraph::DrawHorizontalAxis(wxDC& dc)
{
	wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL, false, wxT("Courier 10 Pitch"));
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

void AudioGraph::DrawVerticalAxis(wxDC& dc,std::vector <double> *verticalAxisVector)
{
	wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL, false, wxT("Courier 10 Pitch"));
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
