#ifndef _HRTF_TEST_DIALOG_H
#define _HRTF_TEST_DIALOG_H

#include <memory>
#include <wx/wx.h>


#include <wx/textctrl.h> //for wxTextCtrl


#include "openalsoftaudioengine.h" //for loading buffer and creating source of sound producer

class HRTFTestDialog : public wxDialog
{

public:
	HRTFTestDialog(const wxString& title,
									OpenAlSoftAudioEngine* audioEngine);


	void OnOk(wxCommandEvent& event );

	void Exit();

	enum
	{
		ID_OK = wxID_HIGHEST + 1,
	};


private:

	wxButton* okButton;

	wxTextCtrl* textBox;

	OpenAlSoftAudioEngine* ptrAudioEngine;

	void initPrivateVariables();

	DECLARE_EVENT_TABLE()

};



#endif
