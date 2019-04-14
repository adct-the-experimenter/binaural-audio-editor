#include "CreateSoundProducerDialog.h"

CreateSoundProducerDialog::CreateSoundProducerDialog(const wxString & title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{

	wxPanel *panel = new wxPanel(this, -1);


	//initialize text fields
	
	wxFloatingPointValidator <double> validator(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator.SetRange(0.00,10.00);     // set allowable range
    
    textFieldName = new wxTextCtrl(panel,-1, "Name", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
    
	textFieldX = new wxTextCtrl(panel,-1, "0.00", 
								wxPoint(95, 60), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldY = new wxTextCtrl(panel,-1, "0.00", 
								wxPoint(95, 80), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldZ = new wxTextCtrl(panel,-1, "0.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT("")); 
	
	//initialize text to the left of fields
	wxStaticText* NameText = new wxStaticText(this, -1, wxT("Name :"), wxPoint(40, 20));
	wxStaticText* positionText = new wxStaticText(this, -1, wxT("Position :"), wxPoint(20, 40));
	wxStaticText* xPositionText = new wxStaticText(this, -1, wxT("X :"), wxPoint(40, 60));
	wxStaticText* yPositionText = new wxStaticText(this, -1, wxT("Y :"), wxPoint(40, 80));
	wxStaticText* zPositionText = new wxStaticText(this, -1, wxT("Z :"), wxPoint(40, 100));
    
    //initialize Ok and Cancel buttons 
	okButton = new wxButton(this, CreateSoundProducerDialog::ID_OK, wxT("Ok"), 
	wxDefaultPosition, wxSize(70, 30));

	cancelButton = new wxButton(this, CreateSoundProducerDialog::ID_CANCEL, wxT("Cancel"), 
	wxDefaultPosition, wxSize(70, 30));

	//Make vertical box to put fields in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	hbox->Add(okButton, 1);
	hbox->Add(cancelButton, 1, wxLEFT, 5);
	
	//add panel of text fields in vertical box
	vbox->Add(panel, 1);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	CreateSoundProducerDialog::initPrivateVariables();
	
	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void CreateSoundProducerDialog::initPrivateVariables()
{
	okClicked = false;
	xPosition = 0.0; yPosition = 0.0; zPosition = 0.0;
}

void CreateSoundProducerDialog::OnOk(wxCommandEvent& event )
{
	 name = textFieldName->GetLineText(0).ToStdString();
	( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	
	okClicked = true;
	
	CreateSoundProducerDialog::Exit();
}

void CreateSoundProducerDialog::OnCancel(wxCommandEvent& event)
{
	okClicked = false;
	CreateSoundProducerDialog::Exit();
}

void CreateSoundProducerDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(cancelButton != nullptr){delete cancelButton;}
	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}
    
    Close( true ); //close window
}

std::string CreateSoundProducerDialog::getNewName(){return name;}

void CreateSoundProducerDialog::getNewPosition(double& x, double& y, double& z)
{
	x = xPosition;
	y = yPosition;
	z = zPosition;
}

bool CreateSoundProducerDialog::OkClickedOn(){ return okClicked;}

//Event table for main frame specific events
BEGIN_EVENT_TABLE(CreateSoundProducerDialog, wxDialog)
    EVT_BUTTON				(ID_OK, CreateSoundProducerDialog::OnOk)
    EVT_BUTTON				(ID_CANCEL, CreateSoundProducerDialog::OnCancel)
END_EVENT_TABLE()
