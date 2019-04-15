#ifndef _EDIT_MULTIPLE_SOUND_PRODUCERS_H
#define _EDIT_MULTIPLE_SOUND_PRODUCERS_H

#include <memory>
#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include "soundproducer.h"


class EditMultipleSoundProducersDialog : public wxDialog
{

public:
	EditMultipleSoundProducersDialog(const wxString& title,std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector);
	
	
	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);
	
	void onApply(wxCommandEvent& event);

	void Exit();
	
	enum 
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_APPLY,
		ID_CANCEL,
		ID_RENAME,
		ID_LISTBOX
	};
	
	
	bool OkClickedOn();
	
private:

	std::vector <std::unique_ptr <SoundProducer> > *sound_producer_vector_ref; //pointer to vector of sound producers to edit
	
	wxButton* okButton;
	wxButton* cancelButton;	
	wxButton* applyButton;
	
	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	
	wxListBox* listbox;
	
	void initPrivateVariables(); 
	
	bool okClicked; //bool to indicate if ok button was clicked on
	
	void SoundProducerSelectedInListBox(wxCommandEvent& event );
	
	DECLARE_EVENT_TABLE()
	
};



#endif
