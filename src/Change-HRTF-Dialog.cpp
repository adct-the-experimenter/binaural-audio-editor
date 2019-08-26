#include "Change-HRTF-Dialog.h"

ChangeHRTFDialog::ChangeHRTFDialog(const wxString & title,OpenAlSoftAudioEngine* audioEngine)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(300, 250), wxRESIZE_BORDER)
{
	selection_index = 0;
	
	ptrAudioEngine = audioEngine;
	
	ChangeHRTFDialog::initPrivateVariables();
	
    wxStaticText* currentHRTFText = new wxStaticText(this, -1, wxT("Currently Selected HRTF :"), wxPoint(20, 40));
	
	wxString thisString(ptrAudioEngine->GetCurrentHRTFSelected());
	wxStaticText* currentHRTFTextHRTF = new wxStaticText(this, -1, thisString, wxPoint(40, 60));
	
	wxBoxSizer *text_vbox = new wxBoxSizer(wxVERTICAL);
	text_vbox->Add(currentHRTFText, 1, wxTOP, 0);
	text_vbox->Add(currentHRTFTextHRTF, 1, wxTOP, 0);
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listbox = new wxListBox(this, ID_LISTBOX,wxPoint(0, 0), wxSize(200, 200)); 
	
	//add all HRTF names to listbox	
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
	
	//initialize change button 
	changeButton = new wxButton(this, ChangeHRTFDialog::ID_CHANGE, wxT("Change"),wxDefaultPosition, wxSize(70, 30));
	
	//Make vertical box to put everything in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//add panel of text fields in vertical box
	vbox->Add(text_vbox, 0, wxBOTTOM, 1);
	vbox->Add(changeButton, 0, wxALL, 1);
	vbox->Add(listbox, 1, wxALL, 0);
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

void ChangeHRTFDialog::OnChange(wxCommandEvent& event)
{
	if(ptrAudioEngine != nullptr)
	{
		if(selection_index != 0)
		{
			int index = selection_index - 1; //decrement to get original index 
			std::string message;
			ptrAudioEngine->SelectThisHRTFByIndex(index,message);
			
			wxString thisString(message);
			wxMessageBox(thisString);
		}
	}
}

void ChangeHRTFDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(listbox != nullptr){delete listbox;}
    if(changeButton != nullptr){delete changeButton;}
    Close( true ); //close window
}

void ChangeHRTFDialog::OnHRTFNameSelected(wxCommandEvent& event)
{
	//increment by 1 so zero is left over for use as undefined
	selection_index = listbox->GetSelection() + 1;
}

//Event table for main frame specific events
BEGIN_EVENT_TABLE(ChangeHRTFDialog, wxDialog)
    EVT_BUTTON				(ID_OK, ChangeHRTFDialog::OnOk)
    EVT_BUTTON				(ID_CHANGE, ChangeHRTFDialog::OnChange)
    EVT_LISTBOX				(ID_LISTBOX,  ChangeHRTFDialog::OnHRTFNameSelected)
END_EVENT_TABLE()
