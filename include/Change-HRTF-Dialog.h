#ifndef _CHANGE_HRTF_DIALOG_H
#define _CHANGE_HRTF_DIALOG_H

#include <memory>
#include <wx/wx.h>


#include <wx/textctrl.h> //for wxTextCtrl


#include <openalsoftaudioengine.h> //for hrtf operations

#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include <wx/msgdlg.h>

class ChangeHRTFDialog : public wxDialog
{

public:
	ChangeHRTFDialog(const wxString& title,
									OpenAlSoftAudioEngine* audioEngine);
	
	
	
	void OnOk(wxCommandEvent& event );

	void Exit();
	
	enum 
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_LISTBOX
	};
	
	
private:
	
	wxButton* okButton;
	
	OpenAlSoftAudioEngine* ptrAudioEngine;
	
	wxListBox* listbox;
	
	void initPrivateVariables(); 
	
	void OnHRTFNameSelected(wxCommandEvent& event);
	
	DECLARE_EVENT_TABLE()
	
};



#endif
