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
	
	//function to return position of new sound producer object
	void getNewPosition(double& x, double& y, double& z);
	
	std::string getNewName();
	
	bool OkClickedOn();
	
private:
	wxButton* okButton;
	wxButton* cancelButton;
	
	
	wxTextCtrl* textFieldName;
	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	
	//Position of New SoundProducer object to be made
	std::string name;
	double xPosition;
	double yPosition;
	double zPosition;
	
	bool okClicked; //bool to indicate if ok button was clicked on
	
	void initPrivateVariables();
	
	DECLARE_EVENT_TABLE()
	
};



#endif
