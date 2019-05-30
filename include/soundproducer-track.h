#ifndef SOUND_PRODUCER_TRACK_H
#define SOUND_PRODUCER_TRACK_H


#include <wx/wx.h>
#include "soundproducer.h"
#include "double-track.h"

//class to manipulate x,y z position of sound producer
class SoundProducerTrack : public Track
{
public:
	SoundProducerTrack(const wxString& title);
	
	void SetReferenceToSoundProducerToManipulate(SoundProducer* thisSoundProducer);
	DoubleTrack* GetReferenceToXTrack();
	DoubleTrack* GetReferenceToYTrack();
	DoubleTrack* GetReferenceToZTrack();
	
	//Double Track related functions
	void SetupAxisForVariable(double& start, double& end, double& resolution, int& numTick);
	
	void OnLeftMouseClick(wxMouseEvent& event);
	void OnRightMouseClick(wxCommandEvent& event);
	
	//Functions inherited from Track
	virtual void InitTrack(wxWindow* parent, std::vector <int> *timeTickVector);

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnScroll(wxScrollEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	
	void SetReferenceToCurrentTimeVariable(double* thisTimeVariable);
	void SetReferenceToTimeTickVector(std::vector <int> *thisVector);
	
	std::vector <int> *GetReferenceToTimeTickVector();
	
	double GetCurrentTime();
	
	//function to call in timer loop, variable to manipulate gets changed here
	virtual void FunctionToCallEveryTimeInTimerLoop();
	
private:
	SoundProducer* soundProducerToManipulatePtr;
	DoubleTrack* xTrack;
	DoubleTrack* yTrack;
	DoubleTrack* zTrack;
	
	//variables to hold temporary values for sound producer position
	double tempX,tempY,tempZ;
	
	wxBoxSizer* main_v_box;
	
};

#endif
