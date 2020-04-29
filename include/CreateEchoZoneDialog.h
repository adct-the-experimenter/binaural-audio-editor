#ifndef CREATE_ECHO_ZONE_H
#define CREATE_ECHO_ZONE_H

#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include "effects-manager.h"



class CreateEchoZoneDialog : public wxDialog
{

public:
	CreateEchoZoneDialog(const wxString& title,EffectsManager* effects_manager);
	

	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);
	
	void OnPreview(wxCommandEvent& event);

	void Exit();


	//function to return position of new sound producer object
	void getNewPosition(double& x, double& y, double& z);

	std::string& getNewName();

	double& getNewWidth();
	
	EchoZoneProperties& getNewProperties();
	
	enum
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_PREVIEW,
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
	wxButton* previewButton;
	
	wxListBox* listboxSoundProducers;
	int spt_selection_index;
	
	//text fields
	wxTextCtrl* textFieldName;

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
	
	//properties of zone to creates
	
	std::string name;
	double xPosition;
	double yPosition;
	double zPosition;
	double width;
	
	EchoZoneProperties properties;
	
	void initPrivateVariables();
	
	bool okClicked;
	
	void SoundProducerTrackSelectedInListBox(wxCommandEvent& event );
	
	

};



#endif
