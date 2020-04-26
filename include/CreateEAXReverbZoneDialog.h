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
	

	void OnOk(wxCommandEvent& event);

	void OnCancel(wxCommandEvent& event);
	
	void OnPreview(wxCommandEvent& event);

	void Exit();


	//function to return position of new sound producer object
	void getNewPosition(double& x, double& y, double& z);

	std::string getNewName();

	double& getNewWidth();
	
	ReverbEAXProperties& getNewProperties();
	
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
	
	
	wxTextCtrl* textFieldName;

	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;
	wxTextCtrl* textFieldWidth;
	
	wxListBox* listboxSoundProducers;
	int spt_selection_index;
	
	void SoundProducerTrackSelectedInListBox(wxCommandEvent& event );
	
	//AL_REVERB_DENSITY, 
	wxTextCtrl* textField_flDensity;
	//AL_REVERB_DIFFUSION, 
	wxTextCtrl* textField_flDiffusion;
	//AL_REVERB_GAIN, 
	wxTextCtrl* textField_flGain;
	//AL_REVERB_GAINHF, 
	wxTextCtrl* textField_flGainHF;
	//AL_EAXREVERB_GAINLF, 
	wxTextCtrl* textField_flGainLF;
	//AL_REVERB_DECAY_TIME, 
	wxTextCtrl* textField_flDecayTime;
	//AL_REVERB_DECAY_HFRATIO, 
	wxTextCtrl* textField_flDecayHFRatio;
	//AL_EAXREVERB_DECAY_LFRATIO, 
	wxTextCtrl* textField_flDecayLFRatio;
	//AL_REVERB_REFLECTIONS_GAIN, 
	wxTextCtrl* textField_flReflectionsGain;
	//AL_REVERB_REFLECTIONS_DELAY, 
	wxTextCtrl* textField_flReflectionsDelay;
	//AL_REVERB_LATE_REVERB_GAIN, 
	wxTextCtrl* textField_flLateReverbGain;
	//AL_REVERB_LATE_REVERB_DELAY, 
	wxTextCtrl* textField_flLateReverbDelay;
	//AL_EAXREVERB_ECHO_TIME, 
	wxTextCtrl* textField_flEchoTime;
	//AL_EAXREVERB_ECHO_DEPTH, 
	wxTextCtrl* textField_flEchoDepth;
	//AL_EAXREVERB_MODULATION_TIME, 
	wxTextCtrl* textField_flModulationTime;
	//AL_EAXREVERB_MODULATION_DEPTH, 
	wxTextCtrl* textField_flModulationDepth;
	//AL_EAXREVERB_HFREFERENCE, 
	wxTextCtrl* textField_flHFReference;
	//AL_EAXREVERB_LFREFERENCE, 
	wxTextCtrl* textField_flLFReference;
	//AL_REVERB_AIR_ABSORPTION_GAINHF, 
	wxTextCtrl* textField_flAirAbsorptionGainHF;
	//AL_REVERB_ROOM_ROLLOFF_FACTOR, 
	wxTextCtrl* textField_flRoomRolloffFactor;

	std::string name;
	double xPosition;
	double yPosition;
	double zPosition;
	double width;
	
	ReverbEAXProperties properties;

	void initPrivateVariables();
	
	bool okClicked;

};



#endif
