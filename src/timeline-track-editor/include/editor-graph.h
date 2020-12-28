#include "wx/wx.h"
#include "wx/sizer.h"

#include "../parameters.h"

#include <vector>
#include <list>
#include <unordered_map>

#include <wx/dcbuffer.h>

typedef std::unordered_map<double, double> DDMap;

class EditorGraph : public wxPanel
{

public:
    EditorGraph(wxWindow* parent);
    
    template <typename T>
    void mouseDownLeftClick(T& vertStart, T& vertEnd, T& vertRes,
							double& time, int& relMouseY, bool& legitValues);
    
    void mouseDownRightClick(double& time,bool& legitValue);
    
    template <typename T>
    void render(wxDC& dc, std::vector <T> *verticalAxisVector);
    
    void SetReferenceToTimeTickVector(std::vector <int> *thisVector); 
	
	//function to return vertical graph value at some time
	//if no value is at that time, then legitValue is false and 0 is returned
	int GetVerticalGraphValueAtThisTime(double& thisTime,bool& legitValue);
	
	template <typename T>
	void PlacePointsFromThisMap(DDMap& thisMap,T& vertStart, T& vertEnd, T& vertRes);
	
private:

	std::vector <int> *timeTickVectorPtr;

	//std::vector <wxPoint> graph_points; //holds points drawn on graph
	
	std::unordered_map <double, wxPoint> map_time; //dictionary to keep track of which time values are occupied
	
	//function to place point on mouse event
	template <typename T>
	void PlacePointByMouse(T& vertStart, T& vertEnd, T& vertRes,
							double& time, int& relMouseY, bool& legitValues);
	
	//function to remove existing point on mouse event
	void RemovePointByMouse(double& time,bool& legitValue);
	
	//function to draw graph_points on graph
	void DrawCurrentPointsOnGraph(wxDC& dc);
	
	//functions to draw axes 
	void DrawHorizontalAxis(wxDC& dc);
	template <typename T>
	void DrawVerticalAxis(wxDC& dc,std::vector <T> *verticalAxisVector);
	
};
