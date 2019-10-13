#ifndef EDIT_MULTIPLE_REVERB_ZONES_DIALOG_H
#define EDIT_MULTIPLE_REVERB_ZONES_DIALOG_H

#include <memory>
#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include "effects-manager.h"

class EditMultipleReverbZonesDialog : public wxDialog
{

public:
	EditMultipleReverbZonesDialog(const wxString& title,EffectsManager* effects_manager);


	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void onApply(wxCommandEvent& event);

	void OnBrowse(wxCommandEvent& event);

	void Exit();

	enum
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_APPLY,
		ID_CANCEL,
		ID_RENAME,
		ID_LISTBOX
	};


private:

	std::vector <std::unique_ptr <SoundProducer> > *sound_producer_vector_ref; //pointer to vector of sound producers to edit

	wxButton* okButton;
	wxButton* cancelButton;
	wxButton* applyButton;

	wxTextCtrl* textFieldName;

	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	wxTextCtrl* textFieldWidth;
	wxTextCtrl* textFieldHeight;


	wxListBox* listbox;

	void initPrivateVariables();

	void ChangeReverbZoneAttributes();

	void ReverbZoneSelectedInListBox(wxCommandEvent& event );

	DECLARE_EVENT_TABLE()

};

#endif
