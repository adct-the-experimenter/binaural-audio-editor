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

void AudioGraph::PlotOneChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data,int sample_rate, double& verticalStart, double& verticalEnd, double& verticalResolution)
{
	double time_resolution = 0.1;
	
	//mulitplying factor to make data show up nicely in graph
	double nice_display_factor = 1;
	
	//calculate number of samples in time_resolution seconds
	int num_samples = time_resolution * sample_rate;
	
	//keep track of every time 0.1 seconds of audio passed
	size_t count = 0;
	
	double min = 0.0f;
	double max = 0.0f;
	
	double min_time_count = 0; //the sample at which minimum value occurs
	double max_time_count = 0; //the sample at which maximum value occurs
	
	double sample_rate_non_int = double(sample_rate);
	
	//for every sample
	for(size_t i=0; i < audio_data->GetSize(); i++)
	{
		//std::cout << "i:" << i << " value:" << audio_data->at(i);
		
		//at end of time_resolution seconds
		if(count == num_samples)
		{	
			//push points for min and max into graph
			
			int xMinTime = (min_time_count / sample_rate) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMinGain = min * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			yMinGain = verticalResolution * round(yMinGain/verticalResolution) * nice_display_factor;
			min_graph_points.push_back(wxPoint(xMinTime,yMinGain));
			
			int xMaxTime = (max_time_count / sample_rate_non_int) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMaxGain = max * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			yMaxGain = verticalResolution * round(yMaxGain/verticalResolution) * nice_display_factor;
			max_graph_points.push_back(wxPoint(xMaxTime,yMaxGain));
			
			//reset min and max
			min = 0;
			max = 0;
			
			count = 0; //reset count
		}
		if(audio_data->GetPointerToDataAtThisSampleIndex(i) != nullptr)
		{
			if(*audio_data->GetPointerToDataAtThisSampleIndex(i) < min 
			&& *audio_data->GetPointerToDataAtThisSampleIndex(i) >= -1)
			{
				min = *audio_data->GetPointerToDataAtThisSampleIndex(i); 
				min_time_count = i;
			}
			
			if(*audio_data->GetPointerToDataAtThisSampleIndex(i) > max 
				&& *audio_data->GetPointerToDataAtThisSampleIndex(i) <= 1)
			{
				max = *audio_data->GetPointerToDataAtThisSampleIndex(i); 
				max_time_count = i;
			}
		}
		
		//increment count
		count++;
	}
}

void AudioGraph::PlotLeftChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data,int sample_rate, double& verticalStart, double& verticalEnd, double& verticalResolution)
{
	double time_resolution = 0.1;
	
	//mulitplying factor to make data show up nicely in graph
	double nice_display_factor = verticalEnd;
	
	//calculate number of samples in time_resolution seconds
	int num_samples = time_resolution * sample_rate;
	
	//keep track of every time 0.1 seconds of audio passed
	size_t count = 0;
	
	double min = 0.0f;
	double max = 0.0f;
	
	double min_time_count = 0; //the sample at which minimum value occurs
	double max_time_count = 0; //the sample at which maximum value occurs
	
	double sample_rate_non_int = double(sample_rate);
	
	//for every other sample, i=0,2,4...
	for(size_t i=0; i < audio_data->GetSize(); i+=2)
	{
		//std::cout << "i:" << i << " value:" << audio_data->at(i);
		
		//time is halved because stereo adds double the number of samples
		
		//at end of time_resolution seconds
		if(count == num_samples)
		{	
			//push points for min and max into graph
			
			int xMinTime = 0.5*(min_time_count / sample_rate) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMinGain = min * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			yMinGain = verticalResolution * round(yMinGain/verticalResolution) * nice_display_factor;
			min_graph_points.push_back(wxPoint(xMinTime,yMinGain));
			
			int xMaxTime = 0.5*(max_time_count / sample_rate_non_int) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMaxGain = max * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			yMaxGain = verticalResolution * round(yMaxGain/verticalResolution) * nice_display_factor;
			max_graph_points.push_back(wxPoint(xMaxTime,yMaxGain));
			
			//reset min and max
			min = 0;
			max = 0;
			
			count = 0; //reset count
		}
		if(audio_data->GetPointerToDataAtThisSampleIndex(i) != nullptr)
		{
			if(*audio_data->GetPointerToDataAtThisSampleIndex(i) < min 
			&& *audio_data->GetPointerToDataAtThisSampleIndex(i) >= -1)
			{
				min = *audio_data->GetPointerToDataAtThisSampleIndex(i); 
				min_time_count = i;
			}
			
			if(*audio_data->GetPointerToDataAtThisSampleIndex(i) > max 
				&& *audio_data->GetPointerToDataAtThisSampleIndex(i) <= 1)
			{
				max = *audio_data->GetPointerToDataAtThisSampleIndex(i); 
				max_time_count = i;
			}
		}
		
		//increment count
		count++;
	}
}										

void AudioGraph::PlotRightChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data,int sample_rate, double& verticalStart, double& verticalEnd, double& verticalResolution)
{
	double time_resolution = 0.1;
	
	//mulitplying factor to make data show up nicely in graph
	double nice_display_factor = verticalEnd;
	
	//calculate number of samples in time_resolution seconds
	int num_samples = time_resolution * sample_rate;
	
	//keep track of every time 0.1 seconds of audio passed
	size_t count = 0;
	
	double min = 0.0f;
	double max = 0.0f;
	
	double min_time_count = 0; //the sample at which minimum value occurs
	double max_time_count = 0; //the sample at which maximum value occurs
	
	double sample_rate_non_int = double(sample_rate);
	
	//for every other sample, i=1,3,5...
	for(size_t i=1; i < audio_data->GetSize(); i+=2)
	{
		//std::cout << "i:" << i << " value:" << audio_data->at(i);
		
		//at end of time_resolution seconds
		if(count == num_samples)
		{	
			//push points for min and max into graph
			
			int xMinTime = 0.5*(min_time_count / sample_rate) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMinGain = min * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			yMinGain = verticalResolution * round(yMinGain/verticalResolution) * nice_display_factor;
			min_graph_points.push_back(wxPoint(xMinTime,yMinGain));
			
			int xMaxTime = 0.5*(max_time_count / sample_rate_non_int) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMaxGain = max * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			yMaxGain = verticalResolution * round(yMaxGain/verticalResolution) * nice_display_factor;
			max_graph_points.push_back(wxPoint(xMaxTime,yMaxGain));
			
			//reset min and max
			min = 0;
			max = 0;
			
			count = 0; //reset count
		}
		if(audio_data->GetPointerToDataAtThisSampleIndex(i) != nullptr)
		{
			if(*audio_data->GetPointerToDataAtThisSampleIndex(i) < min 
			&& *audio_data->GetPointerToDataAtThisSampleIndex(i) >= -1)
			{
				min = *audio_data->GetPointerToDataAtThisSampleIndex(i); 
				min_time_count = i;
			}
			
			if(*audio_data->GetPointerToDataAtThisSampleIndex(i) > max 
				&& *audio_data->GetPointerToDataAtThisSampleIndex(i) <= 1)
			{
				max = *audio_data->GetPointerToDataAtThisSampleIndex(i); 
				max_time_count = i;
			}
		}
		
		//increment count
		count++;
	}
}

void AudioGraph::DrawCurrentDataOnGraph(wxDC& dc)
{
	// draw black circles for max graph points
    dc.SetBrush(*wxBLACK_BRUSH);
    for(size_t i=0; i < max_graph_points.size(); i++)
    {
		dc.DrawCircle( max_graph_points.at(i), 1 );
		if(i != 0)
		{
			//draw line from previous point to next point
			dc.DrawLine( max_graph_points.at(i-1), max_graph_points.at(i) );
		}
	}
	
	//draw blue circles for min graph points
	//dc.SetBrush(*wxBLUE_BRUSH);
    //for(size_t i=0; i < min_graph_points.size(); i++)
    //{
	//	dc.DrawCircle( min_graph_points.at(i), 1 );
	//	if(i != 0)
	//	{
	//		dc.DrawLine(min_graph_points.at(i-1),min_graph_points.at(i));
	//	}
	//}
	
}

void AudioGraph::DrawHorizontalAxis(wxDC& dc)
{
	wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL, false, wxT("Courier 10 Pitch"));
	dc.SetFont(font);
	
	int step = (int) round( int(TRACK_WIDTH) / (int(TIME_TICK_NUM)-1) );
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

void AudioGraph::PlotAudioDataToGraph(std::vector <double> *audio_data, int sample_rate,
										double& verticalStart, double& verticalEnd, double& verticalResolution)
{
	//determines when to stop checking for max and min values
	double time_resolution = 0.5;
	
	//calculate number of samples in time_resolution seconds
	int num_samples = time_resolution * sample_rate;
	
	//mulitplying factor to make data show up nicely in graph
	double nice_display_factor = 0.1;
	
	//keep track of every time 0.1 seconds of audio passed
	size_t count = 0;
	
	double min = 0.0f;
	double max = 0.0f;
	
	double min_time_count = 0; //the sample at which minimum value occurs
	double max_time_count = 0; //the sample at which maximum value occurs
	
	double sample_rate_non_int = double(sample_rate);
	
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
			yMinGain = verticalResolution * round(yMinGain / verticalResolution) * nice_display_factor;
			min_graph_points.push_back(wxPoint(xMinTime,yMinGain));
			
			int xMaxTime = (max_time_count / sample_rate_non_int) * ( double(TRACK_WIDTH) / (double(TIME_END_VALUE) - double(TIME_START_VALUE)) );
			int yMaxGain = max * ( (double(TRACK_HEIGHT)) / (verticalEnd - verticalStart) );
			yMaxGain = verticalResolution * round(yMaxGain / verticalResolution) * nice_display_factor;
			max_graph_points.push_back(wxPoint(xMaxTime,yMaxGain));
			
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
