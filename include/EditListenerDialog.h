#ifndef _EDIT_LISTENER_H
#define _EDIT_LISTENER_H

#include <memory>
#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box


#include <openalsoftaudioengine.h> //for loading buffer and creating source of sound producer
#include "listener.h"

class EditListenerDialog : public wxDialog
{

public:
	EditListenerDialog(const wxString& title, Listener* listener);
	
	
	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);
	
	void onApply(wxCommandEvent& event);
	
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
	
	Listener* ptrListener;
	
	void initPrivateVariables(); 
	
	void ChangeListenerAttributes();
	
	DECLARE_EVENT_TABLE()
	
};



#endif
