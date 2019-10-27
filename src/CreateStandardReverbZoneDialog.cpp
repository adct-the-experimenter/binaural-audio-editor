#include "CreateStandardReverbZoneDialog.h"

CreateStandardReverbZoneDialog::CreateStandardReverbZoneDialog(const wxString& title,EffectsManager* effects_manager) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(400, 600))
{
	okClicked = false;
	
	m_effects_manager_ptr = effects_manager;
	
	//initialize text fields
	
	wxFloatingPointValidator <double> validator(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator.SetRange(-10.00,10.00);     // set allowable range
    
    wxFloatingPointValidator <double> validatorWidth(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorWidth.SetRange(0.01,10.00);     // set allowable range
    
    wxFloatingPointValidator <double> validator0To10(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0To10.SetRange(0.00,10.00);     // set allowable range
    
    wxFloatingPointValidator <double> validator0To1(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0To1.SetRange(0.00,1.00);     // set allowable range
    
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
	
	textFieldWidth = new wxTextCtrl(this,-1, "0.01", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorWidth,          // associate the text box with the desired validator
								wxT("")); 
	
	 
	textField_flDensity = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To1,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flDiffusion = new wxTextCtrl(this,-1, "1.0:", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To1,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flGain = new wxTextCtrl(this,-1, "0.32", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To1,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flGainHF = new wxTextCtrl(this,-1, "0.89", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To1,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flDecayTime = new wxTextCtrl(this,-1, "1.49", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
	
	textField_flDecayHFRatio = new wxTextCtrl(this,-1, "0.83", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
	
	textField_iDecayHFLimit = new wxTextCtrl(this,-1, "decay HF limit:", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
	
	textField_flReflectionsDelay = new wxTextCtrl(this,-1, "0.007", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
	
	textField_flLateReverbGain = new wxTextCtrl(this,-1, "1.26", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
	
	textField_flLateReverbDelay = new wxTextCtrl(this,-1, "0.011", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
								
	textField_flReflectionsGain = new wxTextCtrl(this,-1, "0.05", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
	
	textField_flAirAbsorptionGainHF = new wxTextCtrl(this,-1, "0.994", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,validator0To10,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flRoomRolloffFactor = new wxTextCtrl(this,-1, "room rolloff factor:", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,validator0To10,          // associate the text box with the desired validator
								wxT(""));
	
	
	
	//initialize text to the left of fields
	wxStaticText* NameText = new wxStaticText(this, -1, wxT("Name :"), wxPoint(40, 20));
	wxStaticText* positionText = new wxStaticText(this, -1, wxT("Position :"), wxPoint(20, 40));
	wxStaticText* xPositionText = new wxStaticText(this, -1, wxT("X :"), wxPoint(40, 60));
	wxStaticText* yPositionText = new wxStaticText(this, -1, wxT("Y :"), wxPoint(40, 80));
	wxStaticText* zPositionText = new wxStaticText(this, -1, wxT("Z :"), wxPoint(40, 100));
	wxStaticText* widthText = new wxStaticText(this, -1, wxT("Width :"), wxPoint(40, 120));
    
    wxStaticText* reverbText = new wxStaticText(this, -1, wxT("Reverb Properties"), wxPoint(40, 120));
    wxStaticText* flDensityText = new wxStaticText(this, -1, wxT("density:"), wxPoint(40, 120));
	wxStaticText* flDiffusionText = new wxStaticText(this, -1, wxT("diffusion:"), wxPoint(40, 120));
	wxStaticText* flGainText = new wxStaticText(this, -1, wxT("gain:"), wxPoint(40, 120));
	wxStaticText* flGainHFText = new wxStaticText(this, -1, wxT("gain HF:"), wxPoint(40, 120));
	wxStaticText* flDecayTimeText = new wxStaticText(this, -1, wxT("decay time:"), wxPoint(40, 120));
	wxStaticText* flDecayHFRatioText = new wxStaticText(this, -1, wxT("decay HF ratio:"), wxPoint(40, 120));
	wxStaticText* iDecayHFLimitText = new wxStaticText(this, -1, wxT("decay HF limit:"), wxPoint(40, 120));
	wxStaticText* flLateReverbGainText = new wxStaticText(this, -1, wxT("late reverb gain:"), wxPoint(40, 120));
	wxStaticText* flLateReverbDelayText = new wxStaticText(this, -1, wxT("late reverb delay:"), wxPoint(40, 120));						
	wxStaticText* flReflectionsGainText = new wxStaticText(this, -1, wxT("reflections gain:"), wxPoint(40, 120));
	wxStaticText* flReflectionsDelayText = new wxStaticText(this, -1, wxT("reflections delay:"), wxPoint(40, 120));
	wxStaticText* flAirAbsorptionGainHFText = new wxStaticText(this, -1, wxT("air absorption:"), wxPoint(40, 120));
	wxStaticText* flRoomRolloffFactorText = new wxStaticText(this, -1, wxT("room rolloff factor:"), wxPoint(40, 120)); 

    
    //initialize Ok and Cancel buttons 
	okButton = new wxButton(this, CreateStandardReverbZoneDialog::ID_OK, wxT("Ok"), 
							wxDefaultPosition, wxSize(70, 30));

	cancelButton = new wxButton(this, CreateStandardReverbZoneDialog::ID_CANCEL, wxT("Cancel"), 
								wxDefaultPosition, wxSize(70, 30));
	
	previewButton = new wxButton(this, CreateStandardReverbZoneDialog::ID_PREVIEW, wxT("Preview"), 
								wxDefaultPosition, wxSize(70, 30));

	//Make vertical box to put horizontal boxes in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
	
	hbox5->Add(previewButton,1);
	hbox5->Add(okButton, 1);
	hbox5->Add(cancelButton, 1, wxLEFT, 5);
	
	//add panel of text fields in vertical box
	
	wxBoxSizer *hboxName = new wxBoxSizer(wxHORIZONTAL);
	hboxName->Add(NameText); hboxName->Add(textFieldName);
	
	vbox->Add(hboxName, 1, wxEXPAND | wxALL, 10);
	
	vbox->Add(positionText);
	
	wxBoxSizer *hboxX = new wxBoxSizer(wxHORIZONTAL);
	hboxX->Add(xPositionText); hboxX->Add(textFieldX);
	
	vbox->Add(hboxX,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxY = new wxBoxSizer(wxHORIZONTAL);
	hboxY->Add(yPositionText); hboxY->Add(textFieldY);
	
	vbox->Add(hboxY,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxZ = new wxBoxSizer(wxHORIZONTAL);
	hboxZ->Add(zPositionText); hboxZ->Add(textFieldZ);
	
	vbox->Add(hboxZ,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxWidth = new wxBoxSizer(wxHORIZONTAL);
	hboxWidth->Add(widthText); hboxWidth->Add(textFieldWidth);
	
	vbox->Add(hboxWidth,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(reverbText);
	
	wxBoxSizer *hboxReverbRow1 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow1->Add(flDensityText); hboxReverbRow1->Add(textField_flDensity); 
	hboxReverbRow1->Add(flDiffusionText); hboxReverbRow1->Add(textField_flDiffusion);
	
	vbox->Add(hboxReverbRow1,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow2 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow2->Add(flGainText); hboxReverbRow2->Add(textField_flGain);
	hboxReverbRow2->Add(flGainHFText); hboxReverbRow2->Add(textField_flGainHF);
	
	vbox->Add(hboxReverbRow2,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow3 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow3->Add(flDecayTimeText); hboxReverbRow3->Add(textField_flDecayTime);
	hboxReverbRow3->Add(flDecayHFRatioText); hboxReverbRow3->Add(textField_flDecayHFRatio);
	hboxReverbRow3->Add(iDecayHFLimitText); hboxReverbRow3->Add(textField_iDecayHFLimit);
	
	vbox->Add(hboxReverbRow3,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow4 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow4->Add(flLateReverbGainText); hboxReverbRow4->Add(textField_flLateReverbGain);
	hboxReverbRow4->Add(flLateReverbDelayText); hboxReverbRow4->Add(textField_flLateReverbDelay);
	
	vbox->Add(hboxReverbRow4,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow5 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow5->Add(flReflectionsGainText); hboxReverbRow5->Add(textField_flReflectionsGain);
	hboxReverbRow5->Add(flReflectionsDelayText); hboxReverbRow5->Add(textField_flReflectionsDelay);
	
	vbox->Add(hboxReverbRow5,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow6 = new wxBoxSizer(wxHORIZONTAL);
	
	hboxReverbRow6->Add(flAirAbsorptionGainHFText); hboxReverbRow6->Add(textField_flAirAbsorptionGainHF);
	hboxReverbRow6->Add(flRoomRolloffFactorText); hboxReverbRow6->Add(textField_flRoomRolloffFactor);
	
	vbox->Add(hboxReverbRow6,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(hbox5, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizerAndFit(vbox);
	
	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

std::string& CreateStandardReverbZoneDialog::getNewName(){return name;}

void CreateStandardReverbZoneDialog::getNewPosition(double& x, double& y, double& z)
{
	x = xPosition;
	y = yPosition;
	z = zPosition;
}

double& CreateStandardReverbZoneDialog::getNewWidth(){return width;}
	
ReverbStandardProperties& CreateStandardReverbZoneDialog::getNewProperties(){return properties;}

void CreateStandardReverbZoneDialog::OnOk(wxCommandEvent& event )
{
	okClicked = true;
	
	//save reverb zone properties
	name = textFieldName->GetLineText(0).ToStdString();
	( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	( textFieldWidth->GetLineText(0) ).ToDouble(&width);
	

	( textField_flDensity->GetLineText(0) ).ToDouble(&properties.flDensity);
	( textField_flDiffusion->GetLineText(0) ).ToDouble(&properties.flDiffusion);
	( textField_flGain->GetLineText(0) ).ToDouble(&properties.flGain);
	( textField_flGainHF->GetLineText(0) ).ToDouble(&properties.flGainHF);
	( textField_flDecayTime->GetLineText(0) ).ToDouble(&properties.flDecayTime);
	( textField_flDecayHFRatio->GetLineText(0) ).ToDouble(&properties.flDecayHFRatio);
	( textField_flReflectionsGain->GetLineText(0) ).ToDouble(&properties.flReflectionsGain);
	( textField_flReflectionsDelay->GetLineText(0) ).ToDouble(&properties.flReflectionsDelay);
	( textField_flLateReverbGain->GetLineText(0) ).ToDouble(&properties.flLateReverbGain);
	( textField_flLateReverbDelay->GetLineText(0) ).ToDouble(&properties.flLateReverbDelay);
	( textField_flAirAbsorptionGainHF->GetLineText(0) ).ToDouble(&properties.flAirAbsorptionGainHF);
	( textField_flRoomRolloffFactor->GetLineText(0) ).ToDouble(&properties.flRoomRolloffFactor);
	properties.iDecayHFLimit = wxAtoi( textField_iDecayHFLimit->GetLineText(0) );
	
	CreateStandardReverbZoneDialog::Exit();
}

void CreateStandardReverbZoneDialog::OnPreview(wxCommandEvent& event)
{
	//get sound producer track of first sound producer track
	
	//apply effect to sound producer track
	
	//play for a few seconds
	
	//remove effect from sound producer track
	
}

void CreateStandardReverbZoneDialog::OnCancel(wxCommandEvent& event)
{
	okClicked = false;
	CreateStandardReverbZoneDialog::Exit();
}

void CreateStandardReverbZoneDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(cancelButton != nullptr){delete cancelButton;}
	if(previewButton != nullptr){delete previewButton;}
	
	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}
	if(textFieldWidth != nullptr){ delete textFieldWidth;}
	
 
    Close( true ); //close window
}

bool CreateStandardReverbZoneDialog::OkClicked(){return okClicked;}


//Event table for main frame specific events
BEGIN_EVENT_TABLE(CreateStandardReverbZoneDialog, wxDialog)
    EVT_BUTTON				(ID_OK, CreateStandardReverbZoneDialog::OnOk)
    EVT_BUTTON				(ID_CANCEL, CreateStandardReverbZoneDialog::OnCancel)
    EVT_BUTTON				(ID_PREVIEW, CreateStandardReverbZoneDialog::OnPreview)
    
END_EVENT_TABLE()
