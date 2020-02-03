#include "EditMultipleReverbZonesDialog.h"

EditMultipleReverbZonesDialog::EditMultipleReverbZonesDialog(const wxString& title,EffectsManager* effects_manager)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{
	
	EditMultipleReverbZonesDialog::initPrivateVariables();
	

	//make horizontal box to put names in
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	
	//initialize text fields
	
	wxFloatingPointValidator <double> validator(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator.SetRange(-10.00,10.00);     // set allowable range
    
    wxFloatingPointValidator <double> validatorWidth(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorWidth.SetRange(1.00,10.00);     // set allowable range
    
    wxFloatingPointValidator <double> validator0To10(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0To10.SetRange(0.00,10.00);     // set allowable range
    
    wxFloatingPointValidator <double> validator0To1(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0To1.SetRange(0.00,1.00);     // set allowable range
    
    wxFloatingPointValidator <double> validator0p1To20(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0p1To20.SetRange(0.10,20.00);     // set allowable range
    
    wxFloatingPointValidator <double> validator0p1To2p0(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0p1To2p0.SetRange(0.10,2.00);     // set allowable range
    
    wxFloatingPointValidator <double> validator0p0To0p3(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0p0To0p3.SetRange(0.00,0.30);     // set allowable range
    
    wxFloatingPointValidator <double> validator0p0To0p1(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0p0To0p1.SetRange(0.00,0.10);     // set allowable range
    
    wxFloatingPointValidator <double> validator0p0To3p16(2,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validator0p0To3p16.SetRange(0.00,3.16);     // set allowable range
    
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
	
	textFieldWidth = new wxTextCtrl(this,-1, "2.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorWidth,          // associate the text box with the desired validator
								wxT("")); 
	
	 
	textField_flDensity = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To1,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flDiffusion = new wxTextCtrl(this,-1, "1.0", 
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
								wxTE_PROCESS_ENTER,
								validator0p1To20,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flDecayHFRatio = new wxTextCtrl(this,-1, "0.83", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0p1To2p0,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flReflectionsDelay = new wxTextCtrl(this,-1, "0.007", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0p0To0p3,
								wxT(""));
	
	textField_flLateReverbGain = new wxTextCtrl(this,-1, "1.26", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To10,
								wxT(""));
	
	textField_flLateReverbDelay = new wxTextCtrl(this,-1, "0.011", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0p0To0p1,
								wxT(""));
								
	textField_flReflectionsGain = new wxTextCtrl(this,-1, "0.05", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0p0To3p16,
								wxT(""));
	
	textField_flAirAbsorptionGainHF = new wxTextCtrl(this,-1, "0.994", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To10,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flRoomRolloffFactor = new wxTextCtrl(this,-1, "0.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validator0To10,          // associate the text box with the desired validator
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
	wxStaticText* flLateReverbGainText = new wxStaticText(this, -1, wxT("late reverb gain:"), wxPoint(40, 120));
	wxStaticText* flLateReverbDelayText = new wxStaticText(this, -1, wxT("late reverb delay:"), wxPoint(40, 120));						
	wxStaticText* flReflectionsGainText = new wxStaticText(this, -1, wxT("reflections gain:"), wxPoint(40, 120));
	wxStaticText* flReflectionsDelayText = new wxStaticText(this, -1, wxT("reflections delay:"), wxPoint(40, 120));
	wxStaticText* flAirAbsorptionGainHFText = new wxStaticText(this, -1, wxT("air absorption:"), wxPoint(40, 120));
	wxStaticText* flRoomRolloffFactorText = new wxStaticText(this, -1, wxT("room rolloff factor:"), wxPoint(40, 120)); 

	wxStaticText* spPreviewText = new wxStaticText(this, -1, wxT("Sound Producer on Track To Preview :"), wxPoint(40, 20));
    
    //make horizontal box to put names in
	wxBoxSizer* hboxReverbZones = new wxBoxSizer(wxHORIZONTAL);
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listboxReverbZones = new wxListBox(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 20)); 
	

	
	//add contents of soundproducers to listbox
	for(size_t i = 0; i < effects_manager->GetReferenceToReverbZoneVector()->size(); i++)
	{
		ReverbZone* thisRevZone = effects_manager->GetPointerToReverbZone(i);
		wxString mystring( thisRevZone->GetNameString() );
		listboxReverbZones->Append(mystring);
	}
	
	
	//add listbox to name box
	hboxReverbZones->Add(listboxReverbZones, 1, wxEXPAND | wxALL, 20);
    
    //initialize Ok and Cancel buttons 
	okButton = new wxButton(this, wxID_ANY, wxT("Ok"), 
							wxDefaultPosition, wxSize(70, 30));
	
	okButton->Bind(wxEVT_BUTTON,&EditMultipleReverbZonesDialog::OnOk,this);
	
	applyButton = new wxButton(this, wxID_ANY, wxT("Apply"), 
							wxDefaultPosition, wxSize(70, 30));
	
	applyButton->Bind(wxEVT_BUTTON,&EditMultipleReverbZonesDialog::OnApply,this);
	
	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), 
								wxDefaultPosition, wxSize(70, 30));
	
	cancelButton->Bind(wxEVT_BUTTON,&EditMultipleReverbZonesDialog::OnCancel,this);
	
	previewButton = new wxButton(this, wxID_ANY, wxT("Preview"), 
								wxDefaultPosition, wxSize(70, 30));
	
	previewButton->Bind(wxEVT_BUTTON,&EditMultipleReverbZonesDialog::OnPreview,this);

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
	
	vbox->Add(spPreviewText);
	vbox->Add(hboxReverbZones,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(hbox5, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizerAndFit(vbox);
	
	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void EditMultipleReverbZonesDialog::initPrivateVariables()
{
	m_selection_index = -1;
	sound_producer_vector_ref = nullptr;
	textFieldX = nullptr; textFieldY = nullptr; textFieldZ = nullptr;
	textFieldWidth = nullptr;
	listboxReverbZones = nullptr;
	
	applyButton = nullptr; okButton = nullptr; cancelButton = nullptr;
}


void EditMultipleReverbZonesDialog::ChangeReverbZoneAttributes()
{
	if(m_selection_index != -1)
	{
		
	}
	
	/*
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
			
		}
		
	}
	*/
}

void EditMultipleReverbZonesDialog::OnPreview(wxCommandEvent& event)
{
	
}

void EditMultipleReverbZonesDialog::OnApply(wxCommandEvent& event)
{
	EditMultipleReverbZonesDialog::ChangeReverbZoneAttributes();	
}

void EditMultipleReverbZonesDialog::OnOk(wxCommandEvent& event )
{
	EditMultipleReverbZonesDialog::ChangeReverbZoneAttributes();
	
	EditMultipleReverbZonesDialog::Exit();
}

void EditMultipleReverbZonesDialog::OnCancel(wxCommandEvent& event)
{
	EditMultipleReverbZonesDialog::Exit();
}

void EditMultipleReverbZonesDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(applyButton != nullptr){ delete applyButton;}
	if(cancelButton != nullptr){delete cancelButton;}

	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}

    if(listboxReverbZones != nullptr){delete listboxReverbZones;}
    Close( true ); //close window
}

void EditMultipleReverbZonesDialog::ReverbZoneSelectedInListBox(wxCommandEvent& event )
{
	//std::cout << "\nSelected sound producer! " <<  listboxReverbZones->GetSelection() << " item on the list.\n";
	m_selection_index = listboxReverbZones->GetSelection();
	
	/*
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
	}
	* */
}
