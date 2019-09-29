#ifndef SETUP_SERIAL_DIALOG_H
#define SETUP_SERIAL_DIALOG_H

#include <memory>


#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include <wx/checkbox.h>

#include <wx/msgdlg.h>

#include "listener-external.h"

class SetupSerialDialog : public wxDialog
{

public:
	SetupSerialDialog(const wxString& title,ListenerExternal* listener_ext);


	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void OnSetup(wxCommandEvent& event);

	void Exit();


private:

	wxButton* okButton;
	wxButton* cancelButton;
	wxButton* setupButton;


	wxTextCtrl* textFieldSerialPort;

	ListenerExternal* ptrListenerExternal;

	void initPrivateVariables();

	void ChangeListenerAttributes();

};



#endif
