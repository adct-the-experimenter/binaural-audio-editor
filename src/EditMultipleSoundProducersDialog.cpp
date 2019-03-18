#include "EditMultipleSoundProducersDialog.h"

EditMultipleSoundProducersDialog::EditMultipleSoundProducersDialog(const wxString & title,std::vector <SoundProducer*> *sound_producer_vector)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	
	sound_producer_vector_ref = sound_producer_vector;

	wxPanel * panel = new wxPanel(this, -1);

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	wxListBox* listbox = new wxListBox(panel, ID_LISTBOX, 
									wxPoint(0, 0), wxSize(20, 20)); 
	
	
	
	//add contents to listbox
	/*for(size_t i = 0; i < sound_producer_vector_ref->size(); i++)
	{
		SoundProducer* thisSoundProducer = sound_producer_vector_ref->at(i);
		wxString mystring( thisSoundProducer->GetNameString() );
		listbox->Append(mystring);
	}
	*/
	
	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);


	panel->SetSizer(hbox);
	Center();  
	
	/*
	//initialize text fields
	
	wxFloatingPointValidator <double> validator(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator.SetRange(0.00,10.00);     // set allowable range
    
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
	wxStaticText* positionText = new wxStaticText(this, -1, wxT("Position :"), wxPoint(20, 40));
	wxStaticText* xPositionText = new wxStaticText(this, -1, wxT("X :"), wxPoint(40, 60));
	wxStaticText* yPositionText = new wxStaticText(this, -1, wxT("Y :"), wxPoint(40, 80));
	wxStaticText* zPositionText = new wxStaticText(this, -1, wxT("Z :"), wxPoint(40, 100));
    
	*/
	//initialize Ok and Cancel buttons 
	okButton = new wxButton(this, EditMultipleSoundProducersDialog::ID_OK, wxT("Ok"), 
	wxDefaultPosition, wxSize(70, 30));

	cancelButton = new wxButton(this, EditMultipleSoundProducersDialog::ID_CANCEL, wxT("Cancel"), 
	wxDefaultPosition, wxSize(70, 30));

	//Make vertical box to put fields in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hboxBottom = new wxBoxSizer(wxHORIZONTAL);

	hboxBottom->Add(okButton, 1);
	hboxBottom->Add(cancelButton, 1, wxLEFT, 5);
	
	//add panel of text fields in vertical box
	vbox->Add(panel, 1);
	vbox->Add(hboxBottom, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void EditMultipleSoundProducersDialog::OnOk(wxCommandEvent& event )
{
	//assign value in text field for X position to xPosition
	//( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	//( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	//( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	
	okClicked = true;
	
	EditMultipleSoundProducersDialog::Exit();
}

void EditMultipleSoundProducersDialog::OnCancel(wxCommandEvent& event)
{
	okClicked = false;
	EditMultipleSoundProducersDialog::Exit();
}

void EditMultipleSoundProducersDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(cancelButton != nullptr){delete cancelButton;}
	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}
    
    Close( true ); //close window
}

void EditMultipleSoundProducersDialog::getNewPosition(double& x, double& y, double& z)
{
	//x = xPosition;
	//y = yPosition;
	//z = zPosition;
}

bool EditMultipleSoundProducersDialog::OkClickedOn(){ return okClicked;}
//Event table for main frame specific events
BEGIN_EVENT_TABLE(EditMultipleSoundProducersDialog, wxDialog)
    EVT_BUTTON				(ID_OK, EditMultipleSoundProducersDialog::OnOk)
    EVT_BUTTON				(ID_CANCEL, EditMultipleSoundProducersDialog::OnCancel)
END_EVENT_TABLE()
