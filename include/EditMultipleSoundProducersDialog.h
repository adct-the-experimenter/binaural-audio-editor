#ifndef _EDIT_MULTIPLE_SOUND_PRODUCERS_H
#define _EDIT_MULTIPLE_SOUND_PRODUCERS_H

#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include "soundproducer.h"

class EditMultipleSoundProducersDialog : public wxDialog
{

public:
	EditMultipleSoundProducersDialog(const wxString& title,std::vector <SoundProducer*> *sound_producer_vector);
	
	
	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void Exit();
	
	enum 
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_CANCEL,
		ID_RENAME,
		ID_LISTBOX
	};
	
	//function to return position of new sound producer object
	void getNewPosition(double& x, double& y, double& z);
	
	bool OkClickedOn();
	
private:

	std::vector <SoundProducer*> *sound_producer_vector_ref; //pointer to vector of sound producers to edit

	wxButton* okButton;
	wxButton* cancelButton;	
	
	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	
	
	bool okClicked; //bool to indicate if ok button was clicked on
	
	
	DECLARE_EVENT_TABLE()
	
};



#endif
