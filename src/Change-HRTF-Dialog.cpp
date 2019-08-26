#include "Change-HRTF-Dialog.h"

ChangeHRTFDialog::ChangeHRTFDialog(const wxString & title,OpenAlSoftAudioEngine* audioEngine)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(300, 250), wxRESIZE_BORDER)
{
	
	ptrAudioEngine = audioEngine;
	
	ChangeHRTFDialog::initPrivateVariables();
	
    wxStaticText* currentHRTFText = new wxStaticText(this, -1, wxT("Currently Selected HRTF :"), wxPoint(20, 40));
	
	wxString thisString(ptrAudioEngine->GetCurrentHRTFSelected());
	wxStaticText* currentHRTFTextHRTF = new wxStaticText(this, -1, thisString, wxPoint(40, 60));
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listbox = new wxListBox(this, ID_LISTBOX,wxPoint(0, 0), wxSize(200, 200)); 
	
	//add all HRTF names to listbox
	std::vector <std::string> hrtf_names;
	
	ptrAudioEngine->GetAvailableHRTFNames(&hrtf_names);
	
	if(hrtf_names.size() == 0)
	{
		wxMessageBox( wxT("No HRTF found! Check Test HRTF results. Check OpenAL Soft path for HRTF.") );
	}
	else
	{
		for(size_t i = 0; i < hrtf_names.size(); i++)
		{
			wxString thisName(hrtf_names[i]);
			listbox->Append(thisName);
		}
	}

	//initialize Ok and Cancel buttons 
	okButton = new wxButton(this, ChangeHRTFDialog::ID_OK, wxT("Ok"),wxDefaultPosition, wxSize(70, 30));

	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hboxBottom = new wxBoxSizer(wxHORIZONTAL);

	
	hboxBottom->Add(okButton, 0);
	
	//Make vertical box to put everything in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//add panel of text fields in vertical box
	vbox->Add(currentHRTFText, 1, wxALL, 0);
	vbox->Add(currentHRTFTextHRTF, 1, wxALL, 0);
	vbox->Add(listbox, 1, wxALL, 1);
	vbox->Add(hboxBottom, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
	
	SetSizerAndFit(vbox);

	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void ChangeHRTFDialog::initPrivateVariables()
{
	listbox = nullptr; 
	okButton = nullptr; 
}


void ChangeHRTFDialog::OnOk(wxCommandEvent& event )
{
	ChangeHRTFDialog::Exit();
}


void ChangeHRTFDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(listbox != nullptr){delete listbox;}
    
    Close( true ); //close window
}

void ChangeHRTFDialog::OnHRTFNameSelected(wxCommandEvent& event)
{
	int selection_index = listbox->GetSelection();
}

//Event table for main frame specific events
BEGIN_EVENT_TABLE(ChangeHRTFDialog, wxDialog)
    EVT_BUTTON				(ID_OK, ChangeHRTFDialog::OnOk)
    EVT_LISTBOX				(ID_LISTBOX,  ChangeHRTFDialog::OnHRTFNameSelected)
END_EVENT_TABLE()
