#include "EditMultipleEchoZonesDialog.h"

EditMultipleEchoZonesDialog::EditMultipleEchoZonesDialog(const wxString& title,EffectsManager* effects_manager)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{
	
	EditMultipleEchoZonesDialog::initPrivateVariables();
	
	effects_manager_ptr = effects_manager;

	wxFloatingPointValidator <double> validatorFloat(3,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorFloat.SetRange(-100.00,100.00);     // set allowable range
    
    
	textFieldX = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 60), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldY = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 80), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
								
	textFieldZ = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT("")); 
	
	validatorFloat.SetRange(2.0,30.00);     // set allowable range
	textFieldWidth = new wxTextCtrl(this,-1, "2.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT("")); 
	
	
	validatorFloat.SetRange(0.0,0.207);     // set allowable range
	textField_flDelay = new wxTextCtrl(this,-1, "0.1", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	validatorFloat.SetRange(0.0,0.404);     // set allowable range
	textField_flLRDelay = new wxTextCtrl(this,-1, "0.1", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
								
	validatorFloat.SetRange(0.0,0.99);     // set allowable range
	textField_flDamping = new wxTextCtrl(this,-1, "0.5", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
								
	validatorFloat.SetRange(0.0,1.0);     // set allowable range
	textField_flFeedback = new wxTextCtrl(this,-1, "0.5", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	validatorFloat.SetRange(-1.0,1.0);     // set allowable range
	textField_flSpread = new wxTextCtrl(this,-1, "-1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	
	
	
	
	//initialize text to the left of fields
	wxStaticText* positionText = new wxStaticText(this, -1, wxT("Position :"), wxPoint(20, 40));
	wxStaticText* xPositionText = new wxStaticText(this, -1, wxT("X :"), wxPoint(40, 60));
	wxStaticText* yPositionText = new wxStaticText(this, -1, wxT("Y :"), wxPoint(40, 80));
	wxStaticText* zPositionText = new wxStaticText(this, -1, wxT("Z :"), wxPoint(40, 100));
	wxStaticText* widthText = new wxStaticText(this, -1, wxT("Width :"), wxPoint(40, 120));
    
    wxStaticText* echoText = new wxStaticText(this, -1, wxT("Echo Properties"), wxPoint(40, 120));
    wxStaticText* flDelayText = new wxStaticText(this, -1, wxT("delay:"), wxPoint(40, 120));
	wxStaticText* flLRDelayText = new wxStaticText(this, -1, wxT("LR delay:"), wxPoint(40, 120));
	wxStaticText* flDampingText = new wxStaticText(this, -1, wxT("Damping:"), wxPoint(40, 120));
	wxStaticText* flFeedbackText = new wxStaticText(this, -1, wxT("feedback:"), wxPoint(40, 120));
	wxStaticText* flSpreadText = new wxStaticText(this, -1, wxT("spread:"), wxPoint(40, 120));
	
    wxStaticText* spPreviewText = new wxStaticText(this, -1, wxT("Echo Zone, Sound Track to Preview:"), wxPoint(40, 20));
    
    //make horizontal box to put names in
	wxBoxSizer* hboxEchoZones = new wxBoxSizer(wxHORIZONTAL);
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listboxEchoZones = new wxListBox(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 20)); 
	
	listboxEchoZones->Bind(wxEVT_LISTBOX,&EditMultipleEchoZonesDialog::EchoZoneSelectedInListBox,this);
	
	//add contents of reverb zones to listbox
	for(size_t i = 0; i < effects_manager_ptr->echo_zones_vector.size(); i++)
	{
		EchoZone* thisEchoZone = effects_manager_ptr->GetPointerToEchoZone(i);
					
		wxString mystring( thisEchoZone->GetNameString() );
		listboxEchoZones->Append(mystring);
	}
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listboxSoundProducers = new wxListBox(this, wxID_ANY,wxPoint(0, 0), wxSize(100, 20)); 
	
	listboxSoundProducers->Bind(wxEVT_LISTBOX,&EditMultipleEchoZonesDialog::SoundProducerTrackSelectedInListBox,this);
	
	//add contents of soundproducers to listbox
	if(effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		for(size_t i = 0; i < effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size(); i++)
		{
			SoundProducer* thisSoundProducer =  effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->at(i)->GetReferenceToSoundProducerManipulated();
			if(thisSoundProducer)
			{
				wxString mystring( thisSoundProducer->GetNameString() );
				listboxSoundProducers ->Append(mystring);
			}
		}
	}
	
	
	//add listbox to name box
	hboxEchoZones->Add(listboxEchoZones, 1, wxEXPAND | wxALL, 20);
	hboxEchoZones->Add(listboxSoundProducers, 1, wxEXPAND | wxALL, 20);
    
    //initialize Ok and Cancel buttons 
	okButton = new wxButton(this, wxID_ANY, wxT("Ok"), 
							wxDefaultPosition, wxSize(70, 30));
	
	okButton->Bind(wxEVT_BUTTON,&EditMultipleEchoZonesDialog::OnOk,this);
	
	applyButton = new wxButton(this, wxID_ANY, wxT("Apply"), 
							wxDefaultPosition, wxSize(70, 30));
	
	applyButton->Bind(wxEVT_BUTTON,&EditMultipleEchoZonesDialog::OnApply,this);
	
	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), 
								wxDefaultPosition, wxSize(70, 30));
	
	cancelButton->Bind(wxEVT_BUTTON,&EditMultipleEchoZonesDialog::OnCancel,this);
	
	previewButton = new wxButton(this, wxID_ANY, wxT("Preview"), 
								wxDefaultPosition, wxSize(70, 30));
	
	previewButton->Bind(wxEVT_BUTTON,&EditMultipleEchoZonesDialog::OnPreview,this);

	//Make vertical box to put horizontal boxes in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
	
	hbox5->Add(previewButton,1);
	hbox5->Add(applyButton,1);
	hbox5->Add(okButton, 1);
	hbox5->Add(cancelButton, 1, wxLEFT, 5);
	
	//add panel of text fields in vertical box
	
	vbox->Add(positionText);
	
	wxBoxSizer *hboxPosition = new wxBoxSizer(wxHORIZONTAL);
	hboxPosition->Add(xPositionText); hboxPosition->Add(textFieldX);
	hboxPosition->Add(yPositionText); hboxPosition->Add(textFieldY);
	hboxPosition->Add(zPositionText); hboxPosition->Add(textFieldZ);
	
	int pixelSpaceSeparator = 2;
	
	vbox->Add(hboxPosition,1, wxEXPAND | wxALL, pixelSpaceSeparator);
	
	wxBoxSizer *hboxWidth = new wxBoxSizer(wxHORIZONTAL);
	hboxWidth->Add(widthText); hboxWidth->Add(textFieldWidth);
	
	vbox->Add(hboxWidth,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(echoText);
	
	wxBoxSizer *hboxEchoRow1 = new wxBoxSizer(wxHORIZONTAL);
	hboxEchoRow1->Add(flDelayText); hboxEchoRow1->Add(textField_flDelay); 
	hboxEchoRow1->Add(flLRDelayText); hboxEchoRow1->Add(textField_flLRDelay);
	
	vbox->Add(hboxEchoRow1,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxEchoRow2 = new wxBoxSizer(wxHORIZONTAL);
	hboxEchoRow2->Add(flDampingText); hboxEchoRow2->Add(textField_flDamping);
	
	vbox->Add(hboxEchoRow2,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxEchoRow3 = new wxBoxSizer(wxHORIZONTAL);
	hboxEchoRow3->Add(flFeedbackText); hboxEchoRow3->Add(textField_flFeedback);
	hboxEchoRow3->Add(flSpreadText); hboxEchoRow3->Add(textField_flSpread);
	
	vbox->Add(hboxEchoRow3,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(spPreviewText);
	vbox->Add(hboxEchoZones,1, wxEXPAND | wxALL, pixelSpaceSeparator);
	
	vbox->Add(hbox5, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, pixelSpaceSeparator);

	SetSizerAndFit(vbox);
	
	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

void EditMultipleEchoZonesDialog::initPrivateVariables()
{
	m_selection_index = -1;
	spt_selection_index = -1;
	textFieldX = nullptr; textFieldY = nullptr; textFieldZ = nullptr;
	textFieldWidth = nullptr;
	listboxEchoZones = nullptr;
	
	applyButton = nullptr; okButton = nullptr; cancelButton = nullptr;
}


void EditMultipleEchoZonesDialog::ChangeEchoZoneAttributes()
{
	if(m_selection_index != -1)
	{
		if(effects_manager_ptr->echo_zones_vector.size() > 0)
		{
			
			EchoZone* thisEchoZone = &effects_manager_ptr->echo_zones_vector.at(m_selection_index);
				
			//change position of selected reverb zone based on what is in textfields
			double xPosition, yPosition, zPosition;
			
			( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
			( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
			( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
			
			( textFieldWidth->GetLineText(0) ).ToDouble(&width);
			
			thisEchoZone->SetPositionX(xPosition);
			thisEchoZone->SetPositionY(yPosition);
			thisEchoZone->SetPositionZ(zPosition);
			
			if(width != thisEchoZone->GetWidth())
			{
				thisEchoZone->ChangeWidth(width);
			}
			
			
			( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
			( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
			( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
			( textFieldWidth->GetLineText(0) ).ToDouble(&width);

			( textField_flDelay->GetLineText(0) ).ToDouble(&tempEchoProp.flDelay);
			( textField_flLRDelay->GetLineText(0) ).ToDouble(&tempEchoProp.flLRDelay);
			( textField_flDamping->GetLineText(0) ).ToDouble(&tempEchoProp.flDamping);
			( textField_flFeedback->GetLineText(0) ).ToDouble(&tempEchoProp.flFeedback);
			( textField_flSpread->GetLineText(0) ).ToDouble(&tempEchoProp.flSpread);

			
			thisEchoZone->ChangeEchoZoneProperties(tempEchoProp);
			
			
		}
			
			
			
	}
}
	


void EditMultipleEchoZonesDialog::OnPreview(wxCommandEvent& event)
{
	
	if(effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		
		//get sound producer track of first sound producer track
		std::vector <SoundProducerTrack*> *ref_track_vec = effects_manager_ptr->GetReferenceToSoundProducerTracksVector();
		
		if(spt_selection_index != -1 && m_selection_index != -1)
		{
			SoundProducerTrack* thisTrack = ref_track_vec->at(spt_selection_index);
		
			//if track has a sound producer
			if(thisTrack->GetReferenceToSoundProducerManipulated() != nullptr)
			{
				//Create temporary reverb zone
				EchoZone tempZone;
				
				( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
				( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
				( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
				( textFieldWidth->GetLineText(0) ).ToDouble(&width);
				
				EchoZone* thisEchoZone = &effects_manager_ptr->echo_zones_vector.at(m_selection_index);
					
				
					( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
					( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
					( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
					( textFieldWidth->GetLineText(0) ).ToDouble(&width);

					( textField_flDelay->GetLineText(0) ).ToDouble(&tempEchoProp.flDelay);
					( textField_flLRDelay->GetLineText(0) ).ToDouble(&tempEchoProp.flLRDelay);
					( textField_flDamping->GetLineText(0) ).ToDouble(&tempEchoProp.flDamping);
					( textField_flFeedback->GetLineText(0) ).ToDouble(&tempEchoProp.flFeedback);
					( textField_flSpread->GetLineText(0) ).ToDouble(&tempEchoProp.flSpread);

					tempZone.InitEchoZone(name,xPosition,yPosition,zPosition,width,tempEchoProp);
					
				
				//apply effect to sound producer track
				effects_manager_ptr->ApplyThisEffectZoneEffectToThisTrack(thisTrack, &tempZone);
				
				//play track
				effects_manager_ptr->m_track_manager_ptr->PlayThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//delay for a few seconds 				
				double duration = 4; //seconds
				long endtime = ::wxGetLocalTime() + duration;
				while( ::wxGetLocalTime() < endtime )
				{	
					::wxMilliSleep(100);
				}
				
				//pause instead of stop to avoid crash with stopping source with effect applied
				effects_manager_ptr->m_track_manager_ptr->PauseThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//remove effect from sound producer track
				effects_manager_ptr->RemoveEffectFromThisTrack(thisTrack);
				
				//free effect
				tempZone.FreeEffects();
				
			}
				
				
				
			
			else
			{
				wxMessageBox( wxT("Create a soundproducer. Set it to a track. Load audio to it with browse button!") );	
			}
		}
		else
		{
			wxMessageBox( wxT("Select a soundproducer!") );
		}
	}
}

void EditMultipleEchoZonesDialog::OnApply(wxCommandEvent& event)
{
	EditMultipleEchoZonesDialog::ChangeEchoZoneAttributes();	
}

void EditMultipleEchoZonesDialog::OnOk(wxCommandEvent& event )
{
	EditMultipleEchoZonesDialog::ChangeEchoZoneAttributes();
	
	EditMultipleEchoZonesDialog::Exit();
}

void EditMultipleEchoZonesDialog::OnCancel(wxCommandEvent& event)
{
	EditMultipleEchoZonesDialog::Exit();
}

void EditMultipleEchoZonesDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(applyButton != nullptr){ delete applyButton;}
	if(cancelButton != nullptr){delete cancelButton;}

	if(textFieldX != nullptr){ delete textFieldX;}
	if(textFieldY != nullptr){ delete textFieldY;}
	if(textFieldZ != nullptr){ delete textFieldZ;}

    if(listboxEchoZones != nullptr){delete listboxEchoZones;}
    Close( true ); //close window
}

void EditMultipleEchoZonesDialog::EchoZoneSelectedInListBox(wxCommandEvent& event )
{
	//std::cout << "\nSelected sound producer! " <<  listboxEchoZones->GetSelection() << " item on the list.\n";
	m_selection_index = listboxEchoZones->GetSelection();
	
	
	if(effects_manager_ptr->echo_zones_vector.size() > 0)
	{
		EchoZone* thisEchoZone = &effects_manager_ptr->echo_zones_vector.at(m_selection_index);
			//wxString mystring( thisSoundProducer->GetNameString() );
			//std::cout << "Sound Producer Name: " << thisSoundProducer->GetNameString() << std::endl;
			
			//reset text fields
			textFieldX->Clear();
			textFieldY->Clear();
			textFieldZ->Clear();
			textFieldWidth->Clear();
			
			//update position text fields to have current position of sound producer selected
			(*textFieldX) << thisEchoZone->GetPositionX();
			(*textFieldY) << thisEchoZone->GetPositionY();
			(*textFieldZ) << thisEchoZone->GetPositionZ();
			(*textFieldWidth) << thisEchoZone->GetWidth();
			
			
			
			
				tempEchoProp = thisEchoZone->GetEchoZoneProperties();
			
				textField_flDelay->Clear();
				(*textField_flDelay) << tempEchoProp.flDelay;
				
				textField_flLRDelay->Clear();
				(*textField_flLRDelay) << tempEchoProp.flLRDelay;
				
				textField_flDamping->Clear();
				(*textField_flDamping) << tempEchoProp.flDamping;
				
				textField_flFeedback->Clear();
				(*textField_flFeedback) << tempEchoProp.flFeedback;
				
				textField_flSpread->Clear();
				(*textField_flSpread) << tempEchoProp.flSpread;
			
		
		
	}
}

void EditMultipleEchoZonesDialog::SoundProducerTrackSelectedInListBox(wxCommandEvent& event )
{
	spt_selection_index = listboxSoundProducers->GetSelection();
}


