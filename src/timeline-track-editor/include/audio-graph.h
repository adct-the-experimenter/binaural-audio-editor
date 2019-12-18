#include "wx/wx.h"
#include "wx/sizer.h"

#include "../parameters.h"

#include <wx/settings.h>

#include <vector>
#include <unordered_map>

#include <wx/dcbuffer.h>

#include <limits>

#include "audio-stream-container.h"

//Class used to only plot audio waveforms

class AudioGraph : public wxPanel
{

public:
    AudioGraph(wxWindow* parent);
    
    void mouseDownLeftClick();
    
    void mouseDownRightClick();
    
    void render(wxDC& dc, std::vector <double> *verticalAxisVector);
    
    void SetReferenceToTimeTickVector(std::vector <int> *thisVector); 
	
	//function to return vertical graph value at some time
	//if no value is at that time, then legitValue is false and 0 is returned
	int GetVerticalGraphValueAtThisTime(double& thisTime,bool& legitValue);
	
	void PlotOneChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data,int sample_rate, double& verticalStart, double& verticalEnd, double& verticalResolution);
	void PlotLeftChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data,int sample_rate, double& verticalStart, double& verticalEnd, double& verticalResolution);
	void PlotRightChannelStreamAudioDataToGraph(AudioStreamContainer* audio_data,int sample_rate, double& verticalStart, double& verticalEnd, double& verticalResolution);
	
	void PlotAudioDataToGraph(std::vector <double> *audio_data, int sample_rate, double& verticalStart, double& verticalEnd, double& verticalResolution);
	
	void ClearGraph();
	
private:

	std::vector <int> *timeTickVectorPtr;
	 
	std::vector <wxPoint> max_graph_points; //holds points drawn on graph
	std::vector <wxPoint> min_graph_points;
	
	//std::unordered_map <double, std::vector<wxPoint>::iterator> map_time; //dictionary to keep track of which time values are occupied
	
	//function to draw graph_points on graph
	void DrawCurrentDataOnGraph(wxDC& dc);
	
	//functions to draw axes 
	void DrawHorizontalAxis(wxDC& dc);

	void DrawVerticalAxis(wxDC& dc,std::vector <double> *verticalAxisVector);
	
};

