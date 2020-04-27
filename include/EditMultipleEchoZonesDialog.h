#ifndef EDIT_MULTIPLE_ECHO_ZONES_DIALOG_H
#define EDIT_MULTIPLE_ECHO_ZONES_DIALOG_H

#include <memory>
#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include "effects-manager.h"

class EditMultipleEchoZonesDialog : public wxDialog
{

public:
	EditMultipleEchoZonesDialog(const wxString& title,EffectsManager* effects_manager);


	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void OnApply(wxCommandEvent& event);

	void OnPreview(wxCommandEvent& event);

	void Exit();


private:

	EffectsManager* effects_manager_ptr; //pointer to vector of sound producers to edit
	
	EchoZoneProperties tempEchoProp;
	//ReverbEAXProperties tempEAXRevProp;
	
	std::string name;
	double xPosition;
	double yPosition;
	double zPosition;
	double width;
	
	wxButton* okButton;
	wxButton* cancelButton;
	wxButton* applyButton;
	wxButton* previewButton;

	int m_selection_index;
	
	wxListBox* listboxEchoZones;

	void initPrivateVariables();

	void ChangeEchoZoneAttributes();

	void EchoZoneSelectedInListBox(wxCommandEvent& event );
	
	wxListBox* listboxSoundProducers;
	int spt_selection_index;
	
	void SoundProducerTrackSelectedInListBox(wxCommandEvent& event );
	
	//text fields

	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	wxTextCtrl* textFieldWidth;
	
	
	
	//AL_ECHO_DELAY
	wxTextCtrl* textField_flDelay;
	//AL_ECHO_LRDELAY
	wxTextCtrl* textField_flLRDelay;
	//AL_ECHO_DAMPING
	wxTextCtrl* textField_flDamping;
	//AL_ECHO_FEEDBACK
	wxTextCtrl* textField_flFeedback;
	//AL_ECHO_SPREAD
	wxTextCtrl* textField_flSpread;

};

#endif
