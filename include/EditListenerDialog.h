#ifndef _EDIT_LISTENER_H
#define _EDIT_LISTENER_H

#include <memory>
#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include <wx/checkbox.h>

#include "listener.h"

class EditListenerDialog : public wxDialog
{

public:
	EditListenerDialog(const wxString& title, Listener* listener);
	
	
	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);
	
	void OnApply(wxCommandEvent& event);
	
	void OnBrowse(wxCommandEvent& event);

	void Exit();
	
	enum 
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_APPLY,
		ID_CANCEL
	};
	
	
private:

	wxButton* okButton;
	wxButton* cancelButton;	
	wxButton* applyButton;
	
	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	

	wxCheckBox* checkBoxFreeRoam;
	bool tempFreeRoamBool; 
	void OnFreeRoamCheckBoxClicked(wxCommandEvent& event);
	
	wxCheckBox* checkBoxExternalDeviceOrientation;
	bool tempExternalDeviceOrientation; 
	void OnExternalDeviceOrientationCheckBoxClicked(wxCommandEvent& event);
	
	Listener* ptrListener;
	
	void initPrivateVariables(); 
	
	void ChangeListenerAttributes();
	
};



#endif
