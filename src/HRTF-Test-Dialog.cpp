#include "HRTF-Test-Dialog.h"

HRTFTestDialog::HRTFTestDialog(const wxString & title,OpenAlSoftAudioEngine* audioEngine)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{
	
	ptrAudioEngine = audioEngine;
	
	HRTFTestDialog::initPrivateVariables();
	
    
	textBox = new wxTextCtrl(this,-1, "", wxPoint(95, 140), wxSize(300,300),
							wxTE_READONLY | wxTE_MULTILINE, wxDefaultValidator,      
							wxT("")); 
	textBox->Clear();
	
	//Test HRTF and put test results in textBox
	ptrAudioEngine->TestHRTF();
	
	wxString thisTestResult(ptrAudioEngine->getHRTFTestResult());
	textBox->WriteText(thisTestResult);
	
	ptrAudioEngine->clear_testHRTFResults();
	

	//initialize Ok and Cancel buttons 
	okButton = new wxButton(this, HRTFTestDialog::ID_OK, wxT("Ok"), 
							wxDefaultPosition, wxSize(70, 30)
							);

	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hboxBottom = new wxBoxSizer(wxHORIZONTAL);

	
	hboxBottom->Add(okButton, 0);
	
	//Make vertical box to put everything in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//add panel of text fields in vertical box
	vbox->Add(textBox, 1, wxEXPAND | wxALL, 10);
	vbox->Add(hboxBottom, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
	
	SetSizerAndFit(vbox);

	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void HRTFTestDialog::initPrivateVariables()
{
	textBox = nullptr; 
	okButton = nullptr; 
}


void HRTFTestDialog::OnOk(wxCommandEvent& event )
{
	HRTFTestDialog::Exit();
}


void HRTFTestDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(textBox != nullptr){delete textBox;}
    
    Close( true ); //close window
}

//Event table for main frame specific events
BEGIN_EVENT_TABLE(HRTFTestDialog, wxDialog)
    EVT_BUTTON				(ID_OK, HRTFTestDialog::OnOk)
END_EVENT_TABLE()
