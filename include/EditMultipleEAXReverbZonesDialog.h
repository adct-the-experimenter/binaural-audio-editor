#ifndef EDIT_MULTIPLE_EAX_REVERB_ZONES_DIALOG_H
#define EDIT_MULTIPLE_EAX_REVERB_ZONES_DIALOG_H

#include <memory>
#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box

#include "effects-manager.h"

class EditMultipleEAXReverbZonesDialog : public wxDialog
{

public:
	EditMultipleEAXReverbZonesDialog(const wxString& title,EffectsManager* effects_manager);


	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void OnApply(wxCommandEvent& event);

	void OnPreview(wxCommandEvent& event);

	void Exit();


private:

	EffectsManager* effects_manager_ptr; //pointer to vector of sound producers to edit
	
	//ReverbStandardProperties tempStandardRevProp;
	ReverbEAXProperties tempEAXRevProp;
	
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
	
	wxListBox* listboxReverbZones;

	void initPrivateVariables();

	void ChangeReverbZoneAttributes();

	void ReverbZoneSelectedInListBox(wxCommandEvent& event );
	
	wxListBox* listboxSoundProducers;
	int spt_selection_index;
	
	void SoundProducerTrackSelectedInListBox(wxCommandEvent& event );
	
	//text fields
	wxTextCtrl* textFieldName;

	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	wxTextCtrl* textFieldWidth;
	
	
	
	//AL_REVERB_DENSITY, 
	wxTextCtrl* textField_flDensity;
	//AL_REVERB_DIFFUSION, 
	wxTextCtrl* textField_flDiffusion;
	//AL_REVERB_GAIN, 
	wxTextCtrl* textField_flGain;
	//AL_REVERB_GAINHF, 
	wxTextCtrl* textField_flGainHF;
	//AL_REVERB_DECAY_TIME, 
	wxTextCtrl* textField_flDecayTime;
	//AL_REVERB_DECAY_HFRATIO, 
	wxTextCtrl* textField_flDecayHFRatio;
	//AL_REVERB_REFLECTIONS_GAIN, 
	wxTextCtrl* textField_flReflectionsGain;
	//AL_REVERB_REFLECTIONS_DELAY, 
	wxTextCtrl* textField_flReflectionsDelay;
	//AL_REVERB_LATE_REVERB_GAIN, 
	wxTextCtrl* textField_flLateReverbGain;
	//AL_REVERB_LATE_REVERB_DELAY, 
	wxTextCtrl* textField_flLateReverbDelay;
	//AL_REVERB_AIR_ABSORPTION_GAINHF, 
	wxTextCtrl* textField_flAirAbsorptionGainHF;
	//AL_REVERB_ROOM_ROLLOFF_FACTOR, 
	wxTextCtrl* textField_flRoomRolloffFactor;

};

#endif
