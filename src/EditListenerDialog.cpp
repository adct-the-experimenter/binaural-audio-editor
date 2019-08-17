#include "EditListenerDialog.h"

EditListenerDialog::EditListenerDialog(const wxString & title, Listener* listener)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{
	
	EditListenerDialog::initPrivateVariables();

	//make horizontal box to put names in
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	
	//make vertical box to put display info to edit
	wxBoxSizer *vboxEdit = new wxBoxSizer(wxVERTICAL);
	
	//initialize text fields
	wxFloatingPointValidator <double> validator(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator.SetRange(-10.00,10.00);     // set allowable range
    
    
    
	textFieldX = new wxTextCtrl(this,-1, "", 
								wxPoint(150, 60), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldY = new wxTextCtrl(this,-1, "", 
								wxPoint(150, 80), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldZ = new wxTextCtrl(this,-1, "", 
								wxPoint(150, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator,          // associate the text box with the desired validator
								wxT(""));
								 
	
	//reset text fields
	textFieldX->Clear();
	textFieldY->Clear();
	textFieldZ->Clear();
	
	
	
	//initialize text to the left of fields
	wxStaticText* positionText = new wxStaticText(this, -1, wxT("Position :"), wxPoint(20, 40));
	wxStaticText* xPositionText = new wxStaticText(this, -1, wxT("X :"), wxPoint(40, 60));
	wxStaticText* yPositionText = new wxStaticText(this, -1, wxT("Y :"), wxPoint(40, 80));
	wxStaticText* zPositionText = new wxStaticText(this, -1, wxT("Z :"), wxPoint(40, 100));
    
    //add textfields to edit box
    vboxEdit->Add(positionText, 0,  wxALL, 1);
    vboxEdit->Add(xPositionText, 0,  wxALL, 1);
	vboxEdit->Add(textFieldX, 0, wxALL, 5);
    vboxEdit->Add(yPositionText, 0,  wxALL, 1);
    vboxEdit->Add(textFieldY, 0, wxALL, 5);
    vboxEdit->Add(zPositionText, 0, wxALL, 1);
    vboxEdit->Add(textFieldZ, 0, wxALL, 5);
    
    //add checkmark box to determine if listener can roam freely in world or is controlled by listener track
	checkBoxFreeRoam = new wxCheckBox(this, wxID_ANY, wxT("Free Roam"), wxDefaultPosition, wxSize(30,30));
	checkBoxFreeRoam->Bind(wxEVT_CHECKBOX, &EditListenerDialog::OnFreeRoamCheckBoxClicked,this);
	
	//add checkmark box to determine if listener orientation is controlled by external device or is controlled by listener track
	checkBoxExternalDeviceOrientation = new wxCheckBox(this, wxID_ANY, wxT("External Device Orientation"), wxDefaultPosition, wxSize(30,30));
	checkBoxExternalDeviceOrientation->Bind(wxEVT_CHECKBOX, &EditListenerDialog::OnExternalDeviceOrientationCheckBoxClicked,this);
	

	
	vboxEdit->Add(checkBoxFreeRoam, 1 , wxEXPAND | wxALL, 1);
	vboxEdit->Add(checkBoxExternalDeviceOrientation, 1 , wxEXPAND | wxALL, 1);
	
	hbox->Add(vboxEdit, 1, wxEXPAND | wxALL, 10);
	
	
	//initialize Ok and Cancel buttons 
	okButton = new wxButton(this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize(70, 30) );
	okButton->Bind(wxEVT_BUTTON, &EditListenerDialog::OnOk,this);
	
	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize(70, 30) );
	cancelButton->Bind(wxEVT_BUTTON, &EditListenerDialog::OnCancel,this);
	
	applyButton = new wxButton(this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxSize(70, 30) );
	applyButton->Bind(wxEVT_BUTTON, &EditListenerDialog::OnApply,this);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hboxBottom = new wxBoxSizer(wxHORIZONTAL);

	hboxBottom->Add(applyButton, 0, wxRIGHT, 5);
	hboxBottom->Add(okButton, 0);
	hboxBottom->Add(cancelButton, 0, wxLEFT, 5);
	
	//Make vertical box to put everything in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//add panel of text fields in vertical box
	vbox->Add(hbox, 1, wxEXPAND | wxALL, 10);
	vbox->Add(hboxBottom, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
	
	SetSizerAndFit(vbox);
	
	//update position text fields and checkbox to have current properties of listener 
	ptrListener = listener;
	if(ptrListener == nullptr){std::cout << "listener pointer is nullptr! \n";}
	else
	{
		(*textFieldX) << ptrListener->getPositionX();
		(*textFieldY) << ptrListener->getPositionY();
		(*textFieldZ) << ptrListener->getPositionZ();
		
		
		tempFreeRoamBool = ptrListener->GetListenerFreeRoamBool();
		checkBoxFreeRoam->SetValue(ptrListener->GetListenerFreeRoamBool());
		
		tempExternalDeviceOrientation = ptrListener->GetListenerExternalDeviceOrientationBool();
		checkBoxExternalDeviceOrientation->SetValue(ptrListener->GetListenerExternalDeviceOrientationBool());
	}

	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void EditListenerDialog::initPrivateVariables()
{
	ptrListener = nullptr;
	textFieldX = nullptr; textFieldY = nullptr; textFieldZ = nullptr;
	
	applyButton = nullptr; okButton = nullptr; cancelButton = nullptr;
}

void EditListenerDialog::ChangeListenerAttributes()
{	
	if(ptrListener != nullptr)
	{
		
		//change position of selected sound producer based on what is in textfields
		double xPosition, yPosition, zPosition;
		( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
		( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
		( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
		float x = xPosition;
		float y = yPosition;
		float z = zPosition;
		ptrListener->setPositionX(x);
		ptrListener->setPositionY(y);
		ptrListener->setPositionZ(z);
		
		//change free roam status
		ptrListener->SetListenerFreeRoamBool(tempFreeRoamBool);
		
		//change external device orientation status
		ptrListener->SetListenerExternalDeviceOrientationBool(tempExternalDeviceOrientation);
		
	}
	
}

void EditListenerDialog::OnFreeRoamCheckBoxClicked(wxCommandEvent& event)
{
	tempFreeRoamBool = checkBoxFreeRoam->GetValue();
}


void EditListenerDialog::OnExternalDeviceOrientationCheckBoxClicked(wxCommandEvent& event)
{
	tempExternalDeviceOrientation = checkBoxExternalDeviceOrientation->GetValue();
}

void EditListenerDialog::OnApply(wxCommandEvent& event)
{
	EditListenerDialog::ChangeListenerAttributes();	
}

void EditListenerDialog::OnOk(wxCommandEvent& event )
{
	EditListenerDialog::ChangeListenerAttributes();
	
	EditListenerDialog::Exit();
}

void EditListenerDialog::OnCancel(wxCommandEvent& event)
{
	EditListenerDialog::Exit();
}

void EditListenerDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(applyButton != nullptr){delete applyButton;}
	if(cancelButton != nullptr){delete cancelButton;}
	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}
	if(checkBoxFreeRoam != nullptr){delete checkBoxFreeRoam;}
	if(checkBoxExternalDeviceOrientation != nullptr){delete checkBoxExternalDeviceOrientation;}
    Close( true ); //close window
}
