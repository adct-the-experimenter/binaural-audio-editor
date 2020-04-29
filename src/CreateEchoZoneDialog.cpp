#include "CreateEchoZoneDialog.h"

CreateEchoZoneDialog::CreateEchoZoneDialog(const wxString& title,EffectsManager* effects_manager) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(400, 600))
{
	okClicked = false;
	
	m_effects_manager_ptr = effects_manager;
	
	spt_selection_index = -1;
	
	//initialize text fields
	
	wxFloatingPointValidator <double> validatorFloat(3,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorFloat.SetRange(-100.00,100.00);     // set allowable range
    
    textFieldName = new wxTextCtrl(this,-1, "Name", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
    
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
	wxStaticText* NameText = new wxStaticText(this, -1, wxT("Name :"), wxPoint(40, 20));
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
	
    wxStaticText* spPreviewText = new wxStaticText(this, -1, wxT("Sound Producer on Track To Preview :"), wxPoint(40, 20));
    
    //make horizontal box to put names in
	wxBoxSizer* hboxSoundProducers = new wxBoxSizer(wxHORIZONTAL);
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listboxSoundProducers = new wxListBox(this, ID_LISTBOX, 
							wxPoint(0, 0), wxSize(100, 20)); 
	
	listboxSoundProducers->Bind(wxEVT_LISTBOX,&CreateEchoZoneDialog::SoundProducerTrackSelectedInListBox,this);
	
	//add contents of soundproducers to listbox
	if(m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		for(size_t i = 0; i < m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size(); i++)
		{
			SoundProducer* thisSoundProducer =  m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->at(i)->GetReferenceToSoundProducerManipulated();
			if(thisSoundProducer)
			{
				wxString mystring( thisSoundProducer->GetNameString() );
				listboxSoundProducers ->Append(mystring);
			}
		}
	}
	
	
	//add listbox to name box
	hboxSoundProducers->Add(listboxSoundProducers, 1, wxEXPAND | wxALL, 20);
    
    
    //initialize Ok and Cancel buttons 
	okButton = new wxButton(this, wxID_ANY, wxT("Ok"), 
							wxDefaultPosition, wxSize(70, 30));
	
	okButton->Bind(wxEVT_BUTTON,&CreateEchoZoneDialog::OnOk,this);
	
	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), 
								wxDefaultPosition, wxSize(70, 30));
	
	cancelButton->Bind(wxEVT_BUTTON,&CreateEchoZoneDialog::OnCancel,this);
	
	previewButton = new wxButton(this, wxID_ANY, wxT("Preview"), 
								wxDefaultPosition, wxSize(70, 30));
	
	previewButton->Bind(wxEVT_BUTTON,&CreateEchoZoneDialog::OnPreview,this);
	
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
	vbox->Add(hboxSoundProducers,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(hbox5, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizerAndFit(vbox);
	
	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}

std::string& CreateEchoZoneDialog::getNewName(){return name;}

void CreateEchoZoneDialog::getNewPosition(double& x, double& y, double& z)
{
	x = xPosition;
	y = yPosition;
	z = zPosition;
}

double& CreateEchoZoneDialog::getNewWidth(){return width;}
	
EchoZoneProperties& CreateEchoZoneDialog::getNewProperties(){return properties;}

void CreateEchoZoneDialog::OnOk(wxCommandEvent& event )
{
	okClicked = true;
	
	//save reverb zone properties
	name = textFieldName->GetLineText(0).ToStdString();
	( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	( textFieldWidth->GetLineText(0) ).ToDouble(&width);
	

	( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	( textFieldWidth->GetLineText(0) ).ToDouble(&width);
	
	( textField_flDelay->GetLineText(0) ).ToDouble(&properties.flDelay);
	( textField_flLRDelay->GetLineText(0) ).ToDouble(&properties.flLRDelay);
	( textField_flDamping->GetLineText(0) ).ToDouble(&properties.flDamping);
	( textField_flFeedback->GetLineText(0) ).ToDouble(&properties.flFeedback);
	( textField_flSpread->GetLineText(0) ).ToDouble(&properties.flSpread);
	
	CreateEchoZoneDialog::Exit();
}

void CreateEchoZoneDialog::OnPreview(wxCommandEvent& event)
{
	if(m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		
		//get sound producer track of first sound producer track
		std::vector <SoundProducerTrack*> *ref_track_vec = m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector();
		
		if(spt_selection_index != -1)
		{
			SoundProducerTrack* thisTrack = ref_track_vec->at(spt_selection_index);
		
			//if track has a sound producer
			if(thisTrack->GetReferenceToSoundProducerManipulated() != nullptr)
			{
				//Create temporary reverb zone
				EchoZone tempZone;
				
				name = textFieldName->GetLineText(0).ToStdString();
				( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
				( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
				( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
				( textFieldWidth->GetLineText(0) ).ToDouble(&width);
				
				( textField_flDelay->GetLineText(0) ).ToDouble(&properties.flDelay);
				( textField_flLRDelay->GetLineText(0) ).ToDouble(&properties.flLRDelay);
				( textField_flDamping->GetLineText(0) ).ToDouble(&properties.flDamping);
				( textField_flFeedback->GetLineText(0) ).ToDouble(&properties.flFeedback);
				( textField_flSpread->GetLineText(0) ).ToDouble(&properties.flSpread);
				
				tempZone.InitEchoZone(name,xPosition,yPosition,zPosition,width,properties);
				
				//apply effect to sound producer track
				m_effects_manager_ptr->ApplyThisEffectZoneEffectToThisTrack(thisTrack, &tempZone);
				
				//play track

				m_effects_manager_ptr->m_track_manager_ptr->PlayThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//delay for a few seconds 				
				double duration = 4; //seconds
				long endtime = ::wxGetLocalTime() + duration;
				while( ::wxGetLocalTime() < endtime )
				{	
					::wxMilliSleep(100);
				}
				
				//pause instead of stop to avoid crash with stopping source with effect applied
				m_effects_manager_ptr->m_track_manager_ptr->PauseThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//remove effect from sound producer track
				m_effects_manager_ptr->RemoveEffectFromThisTrack(thisTrack);
				
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

void CreateEchoZoneDialog::SoundProducerTrackSelectedInListBox(wxCommandEvent& event )
{
	spt_selection_index = listboxSoundProducers->GetSelection();
}


void CreateEchoZoneDialog::OnCancel(wxCommandEvent& event)
{
	okClicked = false;
	CreateEchoZoneDialog::Exit();
}

void CreateEchoZoneDialog::Exit()
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

bool CreateEchoZoneDialog::OkClicked(){return okClicked;}

