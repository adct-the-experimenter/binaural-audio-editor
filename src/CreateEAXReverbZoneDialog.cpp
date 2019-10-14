#include "CreateEAXReverbZoneDialog.h"

CreateEAXReverbZoneDialog::CreateEAXReverbZoneDialog(const wxString& title,EffectsManager* effects_manager) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	okClicked = false;
	
	m_effects_manager_ptr = effects_manager;
	
	//initialize text fields
	
	wxFloatingPointValidator <double> validator(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator.SetRange(-10.00,10.00);     // set allowable range
    
    wxFloatingPointValidator <double> validatorWidth(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorWidth.SetRange(0.00,10.00);     // set allowable range
    
    textFieldName = new wxTextCtrl(this,-1, "Name", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
    
	textFieldX = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 60), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldY = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 80), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldZ = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT("")); 
	
	textFieldWidth = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorWidth,          // associate the text box with the desired validator
								wxT("")); 
	
	//initialize text to the left of fields
	wxStaticText* NameText = new wxStaticText(this, -1, wxT("Name :"), wxPoint(40, 20));
	wxStaticText* positionText = new wxStaticText(this, -1, wxT("Position :"), wxPoint(20, 40));
	wxStaticText* xPositionText = new wxStaticText(this, -1, wxT("X :"), wxPoint(40, 60));
	wxStaticText* yPositionText = new wxStaticText(this, -1, wxT("Y :"), wxPoint(40, 80));
	wxStaticText* zPositionText = new wxStaticText(this, -1, wxT("Z :"), wxPoint(40, 100));
	wxStaticText* widthText = new wxStaticText(this, -1, wxT("Width :"), wxPoint(40, 120));
    

    
    //initialize Ok and Cancel buttons 
	okButton = new wxButton(this, CreateEAXReverbZoneDialog::ID_OK, wxT("Ok"), 
							wxDefaultPosition, wxSize(70, 30));

	cancelButton = new wxButton(this, CreateEAXReverbZoneDialog::ID_CANCEL, wxT("Cancel"), 
								wxDefaultPosition, wxSize(70, 30));

	//Make vertical box to put horizontal boxes in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);

	hbox5->Add(okButton, 1);
	hbox5->Add(cancelButton, 1, wxLEFT, 5);
	
	//add panel of text fields in vertical box
	
	wxBoxSizer *hboxName = new wxBoxSizer(wxHORIZONTAL);
	hboxName->Add(NameText);
	hboxName->Add(textFieldName);
	
	vbox->Add(hboxName, 1, wxEXPAND | wxALL, 10);
	
	vbox->Add(positionText);
	
	wxBoxSizer *hboxX = new wxBoxSizer(wxHORIZONTAL);
	hboxX->Add(xPositionText);
	hboxX->Add(textFieldX);
	
	vbox->Add(hboxX,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxY = new wxBoxSizer(wxHORIZONTAL);
	hboxY->Add(yPositionText);
	hboxY->Add(textFieldY);
	
	vbox->Add(hboxY,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxZ = new wxBoxSizer(wxHORIZONTAL);
	hboxZ->Add(zPositionText);
	hboxZ->Add(textFieldZ);
	
	vbox->Add(hboxZ,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxWidth = new wxBoxSizer(wxHORIZONTAL);
	hboxWidth->Add(widthText);
	hboxWidth->Add(textFieldWidth);
	
	vbox->Add(hboxWidth,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(hbox5, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizerAndFit(vbox);
	
	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

std::string CreateEAXReverbZoneDialog::getNewName(){return name;}

void CreateEAXReverbZoneDialog::getNewPosition(double& x, double& y, double& z)
{
	x = xPosition;
	y = yPosition;
	z = zPosition;
}

void CreateEAXReverbZoneDialog::OnOk(wxCommandEvent& event )
{
	okClicked = true;
	
	name = textFieldName->GetLineText(0).ToStdString();
	( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	
	CreateEAXReverbZoneDialog::Exit();
}

void CreateEAXReverbZoneDialog::OnCancel(wxCommandEvent& event)
{
	
	CreateEAXReverbZoneDialog::Exit();
}

void CreateEAXReverbZoneDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(cancelButton != nullptr){delete cancelButton;}
	
	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}
	if(textFieldWidth != nullptr){ delete textFieldWidth;}
	
 
    Close( true ); //close window
}

bool CreateEAXReverbZoneDialog::OkClicked(){return okClicked;}


//Event table for main frame specific events
BEGIN_EVENT_TABLE(CreateEAXReverbZoneDialog, wxDialog)
    EVT_BUTTON				(ID_OK, CreateEAXReverbZoneDialog::OnOk)
    EVT_BUTTON				(ID_CANCEL, CreateEAXReverbZoneDialog::OnCancel)
    
END_EVENT_TABLE()
