#ifndef _EDIT_MULTIPLE_SOUND_PRODUCERS_H
#define _EDIT_MULTIPLE_SOUND_PRODUCERS_H

#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl

class EditMultipleSoundProducersDialog : public wxDialog
{

public:
	EditMultipleSoundProducersDialog(const wxString& title);
	
	
	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void Exit();
	
	enum 
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_CANCEL
	};
	
	//function to return position of new sound producer object
	void setNewPosition(double& x, double& y, double& z);
	
	bool OkClickedOn();
	
private:
	wxButton* okButton;
	wxButton* cancelButton;
	
	
	
	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	
	//Position of New SoundProducer object to be made
	double xPosition;
	double yPosition;
	double zPosition;
	
	bool okClicked; //bool to indicate if ok button was clicked on
	
	void initPrivateVariables();
	
	DECLARE_EVENT_TABLE()
	
};



#endif
