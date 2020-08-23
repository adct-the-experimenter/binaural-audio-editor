#include "CreateSoundProducerDialog.h"

CreateSoundProducerDialog::CreateSoundProducerDialog(const wxString & title, OpenAlSoftAudioEngine* audioEngine)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	
	ptrAudioEngine = audioEngine;

	//initialize text fields
	
	wxFloatingPointValidator <double> validator(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator.SetRange(-10.00,10.00);     // set allowable range
    
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
	
	//textFieldSoundFilePath = new wxTextCtrl(this,-1, "", 
	//							wxPoint(95, 140), wxSize(80,20),
	//							wxTE_READONLY, wxDefaultValidator,       
	//							wxT("")); 
	
	//initialize text to the left of fields
	wxStaticText* NameText = new wxStaticText(this, -1, wxT("Name :"), wxPoint(40, 20));
	wxStaticText* positionText = new wxStaticText(this, -1, wxT("Position :"), wxPoint(20, 40));
	wxStaticText* xPositionText = new wxStaticText(this, -1, wxT("X :"), wxPoint(40, 60));
	wxStaticText* yPositionText = new wxStaticText(this, -1, wxT("Y :"), wxPoint(40, 80));
	wxStaticText* zPositionText = new wxStaticText(this, -1, wxT("Z :"), wxPoint(40, 100));
    //wxStaticText* SoundText = new wxStaticText(this, -1, wxT("Sound File :"), wxPoint(20, 120));
    
    //initialize browse button
    //browseButton = new wxButton(this, CreateSoundProducerDialog::ID_BROWSE, wxT("Browse"), 
	//						wxPoint(110,140), wxSize(70, 30));
    
    //initialize Ok and Cancel buttons 
	okButton = new wxButton(this, CreateSoundProducerDialog::ID_OK, wxT("Ok"), 
							wxDefaultPosition, wxSize(70, 30));

	cancelButton = new wxButton(this, CreateSoundProducerDialog::ID_CANCEL, wxT("Cancel"), 
								wxDefaultPosition, wxSize(70, 30));
								
	//add checkmark box to determine if sound producer can roam freely in world or is controlled by sound producer track
	checkBoxFreeRoam = new wxCheckBox(this, wxID_ANY, wxT("Free Roam"), wxDefaultPosition, wxSize(30,30));

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
	
	vbox->Add(checkBoxFreeRoam,1, wxEXPAND | wxALL, 10);
	
	//wxBoxSizer *hboxSoundFile = new wxBoxSizer(wxHORIZONTAL);
	//hboxSoundFile->Add(SoundText);
	//hboxSoundFile->Add(textFieldSoundFilePath);
	//hboxSoundFile->Add(browseButton);
	
	//vbox->Add(hboxSoundFile,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(hbox5, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizerAndFit(vbox);

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
	soundFilePath = "";
	buffer = 0;
}

void CreateSoundProducerDialog::OnOk(wxCommandEvent& event )
{
	name = textFieldName->GetLineText(0).ToStdString();
	( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	tempFreeRoamBool = checkBoxFreeRoam->GetValue();
	
	okClicked = true;
	
	CreateSoundProducerDialog::Exit();
}

void CreateSoundProducerDialog::OnBrowse(wxCommandEvent& event)
{
	wxFileDialog fileDlg(this, _("Choose the WAV file"), wxEmptyString, wxEmptyString, _("WAV file|*.wav|All files|*.*"));
	if (fileDlg.ShowModal() == wxID_OK)
	{
		wxString path = fileDlg.GetPath();
		//use this path in your app
		soundFilePath = std::string(path.mb_str());
		std::cout << "Sound file path:" << soundFilePath << std::endl;
		
		ALuint tempBuffer;
		//load sound file
		ptrAudioEngine->loadSound(&tempBuffer,soundFilePath);
		
		//if error in loading, show a message box 
		if( tempBuffer == 0)
		{
			std::cout << soundFilePath << "did not load successfully! \n";
		}
		//if successfuly
		else
		{
			//put sound file path string into textfieldSoundFilePath
			wxString thisPath(soundFilePath);
			textFieldSoundFilePath->WriteText(thisPath) ;
			//save to buffer
			buffer = tempBuffer;
		} 
	}   
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

std::string& CreateSoundProducerDialog::getSoundFilePath(){return soundFilePath;}

void CreateSoundProducerDialog::getNewPosition(double& x, double& y, double& z)
{
	x = xPosition;
	y = yPosition;
	z = zPosition;
}

ALuint& CreateSoundProducerDialog::getBuffer(){return buffer;}

bool CreateSoundProducerDialog::OkClickedOn(){ return okClicked;}

bool CreateSoundProducerDialog::getFreeRoamBool(){return tempFreeRoamBool;}

//Event table for main frame specific events
BEGIN_EVENT_TABLE(CreateSoundProducerDialog, wxDialog)
    EVT_BUTTON				(ID_OK, CreateSoundProducerDialog::OnOk)
    EVT_BUTTON				(ID_CANCEL, CreateSoundProducerDialog::OnCancel)
    EVT_BUTTON				(ID_BROWSE, CreateSoundProducerDialog::OnBrowse)
END_EVENT_TABLE()
