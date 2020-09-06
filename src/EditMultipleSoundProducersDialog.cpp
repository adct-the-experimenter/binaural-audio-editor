#include "EditMultipleSoundProducersDialog.h"

EditMultipleSoundProducersDialog::EditMultipleSoundProducersDialog(const wxString & title,
																	std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector,
																	OpenAlSoftAudioEngine* audioEngine)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{
	
	ptrAudioEngine = audioEngine;
	
	EditMultipleSoundProducersDialog::initPrivateVariables();
	
	sound_producer_vector_ref = sound_producer_vector;

	//make horizontal box to put names in
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listbox = new wxListBox(this, ID_LISTBOX, 
							wxPoint(0, 0), wxSize(100, 20)); 
	

	
	//add contents of soundproducers to listbox
	for(size_t i = 0; i < sound_producer_vector_ref->size(); i++)
	{
		SoundProducer* thisSoundProducer = sound_producer_vector_ref->at(i).get();
		wxString mystring( thisSoundProducer->GetNameString() );
		listbox->Append(mystring);
	}
	
	//add listbox to name box
	hbox->Add(listbox, 1, wxEXPAND | wxALL, 20);

	
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
								
	//add checkmark box to determine if sound producer can roam freely in world or is controlled by sound producer track
	checkBoxFreeRoam = new wxCheckBox(this, wxID_ANY, wxT("Free Roam"), wxDefaultPosition, wxSize(30,30));
	
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
	
	hbox->Add(vboxEdit, 1, wxEXPAND | wxALL, 10);
	
	//initialize Ok and Cancel buttons 
	okButton = new wxButton(this, EditMultipleSoundProducersDialog::ID_OK, wxT("Ok"), 
	wxDefaultPosition, wxSize(70, 30)
							);

	cancelButton = new wxButton(this, EditMultipleSoundProducersDialog::ID_CANCEL, wxT("Cancel"), 
	wxDefaultPosition, wxSize(70, 30)
								);
	
	applyButton = new wxButton(this, EditMultipleSoundProducersDialog::ID_APPLY, wxT("Apply"), 
	wxDefaultPosition, wxSize(70, 30)
								);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hboxBottom = new wxBoxSizer(wxHORIZONTAL);

	hboxBottom->Add(applyButton, 0, wxRIGHT, 5);
	hboxBottom->Add(okButton, 0);
	hboxBottom->Add(cancelButton, 0, wxLEFT, 5);
	
	//Make vertical box to put everything in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//add panel of text fields in vertical box
	vbox->Add(hbox, 1, wxEXPAND | wxALL, 1);
	vbox->Add(checkBoxFreeRoam, 1, wxEXPAND | wxALL, 1);
	vbox->Add(hboxBottom, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 1);
	
	SetSizerAndFit(vbox);

	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void EditMultipleSoundProducersDialog::initPrivateVariables()
{
	sound_producer_vector_ref = nullptr;
	textFieldX = nullptr; textFieldY = nullptr; textFieldZ = nullptr;
	listbox = nullptr;
	
	applyButton = nullptr; okButton = nullptr; cancelButton = nullptr;
	soundFilePath = " ";
	buffer = 0;
}

void EditMultipleSoundProducersDialog::ChangeSoundProducerAttributes()
{
	int selection_index = listbox->GetSelection();
	
	if(sound_producer_vector_ref != nullptr)
	{
		if(sound_producer_vector_ref->size() > 0)
		{
			SoundProducer* thisSoundProducer = sound_producer_vector_ref->at(selection_index).get();
		
			//change position of selected sound producer based on what is in textfields
			double xPosition, yPosition, zPosition;
			( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
			( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
			( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
			thisSoundProducer->SetPositionX(xPosition);
			thisSoundProducer->SetPositionY(yPosition);
			thisSoundProducer->SetPositionZ(zPosition);
			//change sound attributes, if they have been changed in menu
			if(buffer != 0){thisSoundProducer->setBuffer(buffer);}
			//if(soundFilePath != " "){thisSoundProducer->setFilepathToSound(soundFilePath);}
			
			thisSoundProducer->SetFreeRoamBool(checkBoxFreeRoam->GetValue());
		}
		
	}
	
}

void EditMultipleSoundProducersDialog::onApply(wxCommandEvent& event)
{
	EditMultipleSoundProducersDialog::ChangeSoundProducerAttributes();	
}

void EditMultipleSoundProducersDialog::OnOk(wxCommandEvent& event )
{
	EditMultipleSoundProducersDialog::ChangeSoundProducerAttributes();
	
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
	//if(browseButton != nullptr){delete browseButton;}
	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}
    //if(textFieldSoundFilePath != nullptr){delete textFieldSoundFilePath;}
    if(listbox != nullptr){delete listbox;}
    Close( true ); //close window
}

void EditMultipleSoundProducersDialog::SoundProducerSelectedInListBox(wxCommandEvent& event )
{
	//std::cout << "\nSelected sound producer! " <<  listbox->GetSelection() << " item on the list.\n";
	int selection_index = listbox->GetSelection();
	
	if(sound_producer_vector_ref != nullptr)
	{
		SoundProducer* thisSoundProducer = sound_producer_vector_ref->at(selection_index).get();
		//wxString mystring( thisSoundProducer->GetNameString() );
		//std::cout << "Sound Producer Name: " << thisSoundProducer->GetNameString() << std::endl;
		
		//reset text fields
		textFieldX->Clear();
		textFieldY->Clear();
		textFieldZ->Clear();
		
		//textFieldSoundFilePath->Clear();
		
		//update position text fields to have current position of sound producer selected
		(*textFieldX) << thisSoundProducer->GetPositionX();
		(*textFieldY) << thisSoundProducer->GetPositionY();
		(*textFieldZ) << thisSoundProducer->GetPositionZ();
		//wxString thisPath(thisSoundProducer->getFilepathToSound());
		//textFieldSoundFilePath->WriteText(thisPath);
		checkBoxFreeRoam->SetValue(thisSoundProducer->GetFreeRoamBool());
	}
}

bool EditMultipleSoundProducersDialog::OkClickedOn(){ return okClicked;}

//Event table for main frame specific events
BEGIN_EVENT_TABLE(EditMultipleSoundProducersDialog, wxDialog)
    EVT_BUTTON				(ID_OK, EditMultipleSoundProducersDialog::OnOk)
    EVT_BUTTON				(ID_CANCEL, EditMultipleSoundProducersDialog::OnCancel)
    EVT_BUTTON				(ID_APPLY, EditMultipleSoundProducersDialog::onApply)
    EVT_LISTBOX				(ID_LISTBOX,  EditMultipleSoundProducersDialog::SoundProducerSelectedInListBox)
    
    //EVT_BUTTON				(ID_BROWSE, EditMultipleSoundProducersDialog::OnBrowse)
END_EVENT_TABLE()
