#ifndef SOUND_PRODUCER_TRACK_H
#define SOUND_PRODUCER_TRACK_H


#include <wx/wx.h>
#include "soundproducer.h"
#include "double-track.h"

#include <memory>
#include <wx/combobox.h>
#include <wx/arrstr.h>

//class to manipulate x,y z position of sound producer
class SoundProducerTrack : public Track
{
public:
	SoundProducerTrack(const wxString& title);
	
	
	DoubleTrack* GetReferenceToXTrack();
	DoubleTrack* GetReferenceToYTrack();
	DoubleTrack* GetReferenceToZTrack();
	
	//SoundProducer Editing
	void SetReferenceToSoundProducerToManipulate(SoundProducer* thisSoundProducer);
	void SetReferenceToSoundProducerVector(std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector);
	
	void AddRecentSoundProducerMadeToTrack();
	void RemoveSoundProducerFromTrack(SoundProducer* thisSoundProducer);
	
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
	
	wxComboBox* m_combo_box;
	
	std::vector <std::unique_ptr <SoundProducer> > *sound_producer_vector_ref; //pointer to vector of sound producers to edit
	//list of names for combo box
	wxArrayString soundproducers_to_edit_wxstring;
	
	std::unordered_map <std::string, std::vector <std::unique_ptr <SoundProducer> >::iterator> map_soundproducer; //dictionary to keep track of which wxstring associated with index
	
	void OnSelectedSoundProducerInComboBox(wxCommandEvent& event);
};

#endif
