#ifndef CREATE_EAX_REVERB_ZONE_H
#define CREATE_EAX_REVERB_ZONE_H

#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl

#include "effects-manager.h"


class CreateEAXReverbZoneDialog : public wxDialog
{

public:
	CreateEAXReverbZoneDialog(const wxString& title,EffectsManager* effects_manager);
	

	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void Exit();


	//function to return position of new sound producer object
	void getNewPosition(double& x, double& y, double& z);

	std::string getNewName();

	double& getNewWidth();
	
	ReverbEAXProperties& getNewProperties();
	
	enum
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_APPLY,
		ID_CANCEL,
		ID_RENAME,
		ID_LISTBOX
	};
	
	bool OkClicked();
	
private:
	EffectsManager* m_effects_manager_ptr;
	
	wxButton* okButton;
	wxButton* cancelButton;

	wxTextCtrl* textFieldName;

	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	wxTextCtrl* textFieldWidth;

	

	std::string name;
	double xPosition;
	double yPosition;
	double zPosition;
	double width;
	
	ReverbEAXProperties m_properties;
	

	void initPrivateVariables();
	
	bool okClicked;
	
	DECLARE_EVENT_TABLE()

};



#endif
