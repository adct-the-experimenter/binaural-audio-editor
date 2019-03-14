#ifndef _CREATE_SOUND_PRODUCER_H
#define _CREATE_SOUND_PRODUCER_H

#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl

class CreateSoundProducerDialog : public wxDialog
{

public:
	CreateSoundProducerDialog(const wxString& title);
	
	
	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void Exit();
	
	enum 
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_CANCEL
	};
	
private:
	wxButton* okButton;
	wxButton* cancelButton;
	
	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	
	double xPosition;
	double yPosition;
	double zPosition;
	
	DECLARE_EVENT_TABLE()
	
};



#endif
