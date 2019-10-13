#include "CreateReverbZoneDialog.h"

CreateReverbZoneDialog::CreateReverbZoneDialog(const wxString& title,EffectsManager* effects_manager) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	m_effects_manager_ptr = effects_manager;
}

void CreateReverbZoneDialog::OnOk(wxCommandEvent& event )
{
	
	CreateReverbZoneDialog::Exit();
}

void CreateReverbZoneDialog::OnCancel(wxCommandEvent& event)
{
	
	CreateReverbZoneDialog::Exit();
}

void CreateReverbZoneDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(cancelButton != nullptr){delete cancelButton;}
	
	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}
	if(textFieldWidth != nullptr){ delete textFieldWidth;}
	if(textFieldHeight != nullptr){ delete textFieldHeight;}
	
 
    Close( true ); //close window
}


//Event table for main frame specific events
BEGIN_EVENT_TABLE(CreateReverbZoneDialog, wxDialog)
    EVT_BUTTON				(ID_OK, CreateReverbZoneDialog::OnOk)
    EVT_BUTTON				(ID_CANCEL, CreateReverbZoneDialog::OnCancel)
    
    //EVT_BUTTON				(ID_BROWSE, EditMultipleSoundProducersDialog::OnBrowse)
END_EVENT_TABLE()
