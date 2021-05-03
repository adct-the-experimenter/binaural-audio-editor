#include "CreateEAXReverbZoneDialog.h"

CreateEAXReverbZoneDialog::CreateEAXReverbZoneDialog(const wxString& title,EffectsManager* effects_manager) : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
	okClicked = false;
	
	m_effects_manager_ptr = effects_manager;
	spt_selection_index = -1;
	
	//initialize text fields
	
	wxFloatingPointValidator <double> validatorFloat(3,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorFloat.SetRange(0.00,10.00);     // set allowable range
    
    
    textFieldName = new wxTextCtrl(this,-1, "Name", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER);
    
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
	
	validatorFloat.SetRange(2.00,30.00);     // set allowable range
	textFieldWidth = new wxTextCtrl(this,-1, "2.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT("")); 
	
	validatorFloat.SetRange(0.0,1.0);     // set allowable range
	textField_flDensity = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flDiffusion = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	
	textField_flGain = new wxTextCtrl(this,-1, "0.32", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flGainHF = new wxTextCtrl(this,-1, "0.89", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flGainLF = new wxTextCtrl(this,-1, "0.89", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	validatorFloat.SetRange(0.00,20.00);     // set allowable range
	textField_flDecayTime = new wxTextCtrl(this,-1, "1.49", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	
	textField_flDecayHFRatio = new wxTextCtrl(this,-1, "0.83", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_flDecayLFRatio = new wxTextCtrl(this,-1, "0.83", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	validatorFloat.SetRange(0.00,3.16);     // set allowable range
	textField_flReflectionsDelay = new wxTextCtrl(this,-1, "0.007", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
	
	validatorFloat.SetRange(0.00,0.3);     // set allowable range
	textField_flReflectionsGain = new wxTextCtrl(this,-1, "0.05", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
	
	validatorFloat.SetRange(0.00,10.0);     // set allowable range
	textField_flLateReverbGain = new wxTextCtrl(this,-1, "1.26", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
	
	validatorFloat.SetRange(0.00,0.1);     // set allowable range
	textField_flLateReverbDelay = new wxTextCtrl(this,-1, "0.011", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));

	validatorFloat.SetRange(0.075,0.25);     // set allowable range
	textField_flEchoTime = new wxTextCtrl(this,-1, "0.2", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));

	validatorFloat.SetRange(0.00,1.0);     // set allowable range							
	textField_flEchoDepth = new wxTextCtrl(this,-1, "0.05", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
    
    validatorFloat.SetRange(0.004,4.0);     // set allowable range									
	textField_flModulationTime = new wxTextCtrl(this,-1, "0.05", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
	
	
	validatorFloat.SetRange(0.00,1.0);     // set allowable range
	textField_flModulationDepth = new wxTextCtrl(this,-1, "0.05", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
	
	validatorFloat.SetRange(1000.00,20000.0);     // set allowable range
	textField_flHFReference = new wxTextCtrl(this,-1, "1000", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
								
    validatorFloat.SetRange(20.0,1000.0);     // set allowable range								
	textField_flLFReference = new wxTextCtrl(this,-1, "250.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,
								wxT(""));
	
	validatorFloat.SetRange(0.892,1.0);     // set allowable range
	textField_flAirAbsorptionGainHF = new wxTextCtrl(this,-1, "0.994", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	validatorFloat.SetRange(0.00,10.0);     // set allowable range
	textField_flRoomRolloffFactor = new wxTextCtrl(this,-1, "0.0", 
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
    
    wxStaticText* reverbText = new wxStaticText(this, -1, wxT("Reverb Properties"), wxPoint(40, 120));
    wxStaticText* flDensityText = new wxStaticText(this, -1, wxT("density:"), wxPoint(40, 120));
	wxStaticText* flDiffusionText = new wxStaticText(this, -1, wxT("diffusion:"), wxPoint(40, 120));
	wxStaticText* flGainText = new wxStaticText(this, -1, wxT("gain:"), wxPoint(40, 120));
	wxStaticText* flGainHFText = new wxStaticText(this, -1, wxT("gain HF:"), wxPoint(40, 120));
	wxStaticText* flGainLFText = new wxStaticText(this, -1, wxT("gain LF:"), wxPoint(40, 120));
	wxStaticText* flDecayTimeText = new wxStaticText(this, -1, wxT("decay time:"), wxPoint(40, 120));
	wxStaticText* flDecayHFRatioText = new wxStaticText(this, -1, wxT("decay HF ratio:"), wxPoint(40, 120));
	wxStaticText* flDecayLFRatioText = new wxStaticText(this, -1, wxT("decay LF ratio:"), wxPoint(40, 120));
	wxStaticText* flLateReverbGainText = new wxStaticText(this, -1, wxT("late reverb gain:"), wxPoint(40, 120));
	wxStaticText* flLateReverbDelayText = new wxStaticText(this, -1, wxT("late reverb delay:"), wxPoint(40, 120));						
	
	wxStaticText* flEchoTimeText = new wxStaticText(this, -1, wxT("echo time:"), wxPoint(40, 120));						
	wxStaticText* flEchoDepthText = new wxStaticText(this, -1, wxT("echo depth:"), wxPoint(40, 120));						
	wxStaticText* flModulationTimeText = new wxStaticText(this, -1, wxT("modulation time:"), wxPoint(40, 120));						
	wxStaticText* flModulationDepthText = new wxStaticText(this, -1, wxT("modulation depth:"), wxPoint(40, 120));						
	wxStaticText* flHFReferenceText = new wxStaticText(this, -1, wxT("HF Reference:"), wxPoint(40, 120));						
	wxStaticText* flLFReferenceText = new wxStaticText(this, -1, wxT("LF Reference:"), wxPoint(40, 120));						
	
	wxStaticText* flReflectionsGainText = new wxStaticText(this, -1, wxT("reflections gain:"), wxPoint(40, 120));
	wxStaticText* flReflectionsDelayText = new wxStaticText(this, -1, wxT("reflections delay:"), wxPoint(40, 120));
	wxStaticText* flAirAbsorptionGainHFText = new wxStaticText(this, -1, wxT("air absorption:"), wxPoint(40, 120));
	wxStaticText* flRoomRolloffFactorText = new wxStaticText(this, -1, wxT("room rolloff factor:"), wxPoint(40, 120)); 

	wxStaticText* spPreviewText = new wxStaticText(this, -1, wxT("Sound Producer on Track To Preview :"), wxPoint(40, 20));
    
    //make horizontal box to put names in
	wxBoxSizer* hboxSoundProducers = new wxBoxSizer(wxHORIZONTAL);
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listboxSoundProducers = new wxListBox(this, ID_LISTBOX, 
							wxPoint(0, 0), wxSize(100, 20)); 
	

	listboxSoundProducers->Bind(wxEVT_LISTBOX,&CreateEAXReverbZoneDialog::SoundProducerTrackSelectedInListBox,this);
	
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
	
	okButton->Bind(wxEVT_BUTTON,&CreateEAXReverbZoneDialog::OnOk,this);
	
	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), 
								wxDefaultPosition, wxSize(70, 30));
	
	cancelButton->Bind(wxEVT_BUTTON,&CreateEAXReverbZoneDialog::OnCancel,this);
	
	previewButton = new wxButton(this, wxID_ANY, wxT("Preview"), 
								wxDefaultPosition, wxSize(70, 30));
	
	previewButton->Bind(wxEVT_BUTTON,&CreateEAXReverbZoneDialog::OnPreview,this);

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
	hboxReverbRow2->Add(flGainLFText); hboxReverbRow2->Add(textField_flGainLF);
	
	vbox->Add(hboxReverbRow2,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow3 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow3->Add(flDecayTimeText); hboxReverbRow3->Add(textField_flDecayTime);
	hboxReverbRow3->Add(flDecayHFRatioText); hboxReverbRow3->Add(textField_flDecayHFRatio);
	hboxReverbRow3->Add(flDecayLFRatioText); hboxReverbRow3->Add(textField_flDecayLFRatio);
	
	vbox->Add(hboxReverbRow3,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow4 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow4->Add(flLateReverbGainText); hboxReverbRow4->Add(textField_flLateReverbGain);
	hboxReverbRow4->Add(flLateReverbDelayText); hboxReverbRow4->Add(textField_flLateReverbDelay);
	
	vbox->Add(hboxReverbRow4,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hBoxEAXRow1 = new wxBoxSizer(wxHORIZONTAL);
	hBoxEAXRow1->Add(flEchoDepthText); hBoxEAXRow1->Add(textField_flEchoDepth);
	hBoxEAXRow1->Add(flEchoTimeText); hBoxEAXRow1->Add(textField_flEchoTime);
	
	vbox->Add(hBoxEAXRow1,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hBoxEAXRow2 = new wxBoxSizer(wxHORIZONTAL);
	hBoxEAXRow2->Add(flModulationDepthText); hBoxEAXRow2->Add(textField_flModulationDepth);
	hBoxEAXRow2->Add(flModulationTimeText); hBoxEAXRow2->Add(textField_flModulationTime);
	
	vbox->Add(hBoxEAXRow2,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hBoxEAXRow3 = new wxBoxSizer(wxHORIZONTAL);
	hBoxEAXRow3->Add(flLFReferenceText); hBoxEAXRow3->Add(textField_flLFReference);
	hBoxEAXRow3->Add(flHFReferenceText); hBoxEAXRow3->Add(textField_flHFReference);
	
	vbox->Add(hBoxEAXRow3,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow5 = new wxBoxSizer(wxHORIZONTAL);
	hboxReverbRow5->Add(flReflectionsGainText); hboxReverbRow5->Add(textField_flReflectionsGain);
	hboxReverbRow5->Add(flReflectionsDelayText); hboxReverbRow5->Add(textField_flReflectionsDelay);
	
	vbox->Add(hboxReverbRow5,1, wxEXPAND | wxALL, 10);
	
	wxBoxSizer *hboxReverbRow6 = new wxBoxSizer(wxHORIZONTAL);
	
	hboxReverbRow6->Add(flAirAbsorptionGainHFText); hboxReverbRow6->Add(textField_flAirAbsorptionGainHF);
	hboxReverbRow6->Add(flRoomRolloffFactorText); hboxReverbRow6->Add(textField_flRoomRolloffFactor);
	
	vbox->Add(hboxReverbRow6,1, wxEXPAND | wxALL, 10);
	
	vbox->Add(spPreviewText);
	vbox->Add(hboxSoundProducers,0, wxEXPAND | wxALL, 10);
	
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

double& CreateEAXReverbZoneDialog::getNewWidth(){return width;}
	
ReverbEAXProperties& CreateEAXReverbZoneDialog::getNewProperties(){return properties;}

void CreateEAXReverbZoneDialog::OnOk(wxCommandEvent& event )
{
	okClicked = true;
	
	name = textFieldName->GetLineText(0).ToStdString();
	( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
	( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
	( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
	( textFieldWidth->GetLineText(0) ).ToDouble(&width);
	
	( textField_flDensity->GetLineText(0) ).ToDouble(&properties.flDensity);
	( textField_flDiffusion->GetLineText(0) ).ToDouble(&properties.flDiffusion);
	( textField_flGain->GetLineText(0) ).ToDouble(&properties.flGain);
	( textField_flGainHF->GetLineText(0) ).ToDouble(&properties.flGainHF);
	( textField_flGainLF->GetLineText(0) ).ToDouble(&properties.flGainLF);
	( textField_flDecayTime->GetLineText(0) ).ToDouble(&properties.flDecayTime);
	( textField_flDecayHFRatio->GetLineText(0) ).ToDouble(&properties.flDecayHFRatio);
	( textField_flDecayLFRatio->GetLineText(0) ).ToDouble(&properties.flDecayLFRatio);
	( textField_flReflectionsGain->GetLineText(0) ).ToDouble(&properties.flReflectionsGain);
	( textField_flReflectionsDelay->GetLineText(0) ).ToDouble(&properties.flReflectionsDelay);
	( textField_flLateReverbGain->GetLineText(0) ).ToDouble(&properties.flLateReverbGain);
	( textField_flLateReverbDelay->GetLineText(0) ).ToDouble(&properties.flLateReverbDelay);
	( textField_flEchoDepth->GetLineText(0) ).ToDouble(&properties.flEchoDepth);
	( textField_flEchoTime->GetLineText(0) ).ToDouble(&properties.flEchoTime);
	( textField_flModulationDepth->GetLineText(0) ).ToDouble(&properties.flModulationDepth);
	( textField_flModulationTime->GetLineText(0) ).ToDouble(&properties.flModulationTime);
	( textField_flHFReference->GetLineText(0) ).ToDouble(&properties.flHFReference);
	( textField_flLFReference->GetLineText(0) ).ToDouble(&properties.flLFReference);
	( textField_flAirAbsorptionGainHF->GetLineText(0) ).ToDouble(&properties.flAirAbsorptionGainHF);
	( textField_flRoomRolloffFactor->GetLineText(0) ).ToDouble(&properties.flRoomRolloffFactor);
	
	CreateEAXReverbZoneDialog::Exit();
}

void CreateEAXReverbZoneDialog::OnPreview(wxCommandEvent& event)
{
	if(m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		if(spt_selection_index != -1)
		{
			//get sound producer track of first sound producer track
			std::vector <SoundProducerTrack*> *ref_track_vec = m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector();
			SoundProducerTrack* thisTrack = ref_track_vec->at(spt_selection_index);
			
			//if track has a sound producer
			if(thisTrack->GetReferenceToSoundProducerManipulated() != nullptr)
			{
				//Create temporary reverb zone
				ReverbZone tempZone;
				
				name = textFieldName->GetLineText(0).ToStdString();
				( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
				( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
				( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
				( textFieldWidth->GetLineText(0) ).ToDouble(&width);
				
				( textField_flDensity->GetLineText(0) ).ToDouble(&properties.flDensity);
				( textField_flDiffusion->GetLineText(0) ).ToDouble(&properties.flDiffusion);
				( textField_flGain->GetLineText(0) ).ToDouble(&properties.flGain);
				( textField_flGainHF->GetLineText(0) ).ToDouble(&properties.flGainHF);
				( textField_flGainLF->GetLineText(0) ).ToDouble(&properties.flGainLF);
				( textField_flDecayTime->GetLineText(0) ).ToDouble(&properties.flDecayTime);
				( textField_flDecayHFRatio->GetLineText(0) ).ToDouble(&properties.flDecayHFRatio);
				( textField_flDecayLFRatio->GetLineText(0) ).ToDouble(&properties.flDecayLFRatio);
				( textField_flReflectionsGain->GetLineText(0) ).ToDouble(&properties.flReflectionsGain);
				( textField_flReflectionsDelay->GetLineText(0) ).ToDouble(&properties.flReflectionsDelay);
				( textField_flLateReverbGain->GetLineText(0) ).ToDouble(&properties.flLateReverbGain);
				( textField_flLateReverbDelay->GetLineText(0) ).ToDouble(&properties.flLateReverbDelay);
				( textField_flEchoDepth->GetLineText(0) ).ToDouble(&properties.flEchoDepth);
				( textField_flEchoTime->GetLineText(0) ).ToDouble(&properties.flEchoTime);
				( textField_flModulationDepth->GetLineText(0) ).ToDouble(&properties.flModulationDepth);
				( textField_flModulationTime->GetLineText(0) ).ToDouble(&properties.flModulationTime);
				( textField_flHFReference->GetLineText(0) ).ToDouble(&properties.flHFReference);
				( textField_flLFReference->GetLineText(0) ).ToDouble(&properties.flLFReference);
				( textField_flAirAbsorptionGainHF->GetLineText(0) ).ToDouble(&properties.flAirAbsorptionGainHF);
				( textField_flRoomRolloffFactor->GetLineText(0) ).ToDouble(&properties.flRoomRolloffFactor);
				
				tempZone.InitEAXReverbZone(name,xPosition,yPosition,zPosition,width,properties);
				
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
			
		}
		else
		{
			wxMessageBox( wxT("Select a sound producer!") );	
		}
	}
	else
	{
		wxMessageBox( wxT("Create a soundproducer. Set it to a track. Load audio to it with browse button!") );
	}
	
}

void CreateEAXReverbZoneDialog::SoundProducerTrackSelectedInListBox(wxCommandEvent& event )
{
	spt_selection_index = listboxSoundProducers->GetSelection();
}


void CreateEAXReverbZoneDialog::OnCancel(wxCommandEvent& event)
{
	
	CreateEAXReverbZoneDialog::Exit();
}

void CreateEAXReverbZoneDialog::Exit()
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

bool CreateEAXReverbZoneDialog::OkClicked(){return okClicked;}
