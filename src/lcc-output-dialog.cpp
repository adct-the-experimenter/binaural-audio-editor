#include "lcc-output-dialog.h"

#include "RtAudio.h"

LCCOutputDialog::LCCOutputDialog(const wxString& title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{	

	//initialize text fields

	wxFloatingPointValidator <double> validatorFloat(3,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorFloat.SetRange(0.00,10.00);     // set allowable range
    
    
    validatorFloat.SetRange(-100.00,100.00);     // set allowable range
	textField_samplerate = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 60), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_inputgain = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 80), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
								
	textField_centergain = new wxTextCtrl(this,-1, "0.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT("")); 
	
	validatorFloat.SetRange(2.00,30.00);     // set allowable range
	textField_endgain = new wxTextCtrl(this,-1, "2.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT("")); 
	
	validatorFloat.SetRange(0.0,1.0);     // set allowable range
	textField_decaygain = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_delay_us = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_inputDevice = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	textField_outputDevice = new wxTextCtrl(this,-1, "1.0", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT(""));
	
	//initialize text to the left of text fields
    wxStaticText* devicesText = new wxStaticText(this, -1, wxT("Devices"), wxPoint(40, 120));
    wxStaticText* inputDeviceText = new wxStaticText(this, -1, wxT("Input Device:"), wxPoint(40, 120));
    wxStaticText* outputDeviceText = new wxStaticText(this, -1, wxT("Output Device:"), wxPoint(40, 120));
    wxStaticText* sampleRateText = new wxStaticText(this, -1, wxT("Sample Rate:"), wxPoint(40, 120));
	wxStaticText* inputGainText = new wxStaticText(this, -1, wxT("Input Gain:"), wxPoint(40, 120));
	wxStaticText* centerGainText = new wxStaticText(this, -1, wxT("Center Gain:"), wxPoint(40, 120));
	wxStaticText* endGainText = new wxStaticText(this, -1, wxT("End Gain:"), wxPoint(40, 120));
	wxStaticText* decayGainText = new wxStaticText(this, -1, wxT("Decay Gain:"), wxPoint(40, 120));
	wxStaticText* delayUSText = new wxStaticText(this, -1, wxT("Delay (microseconds):"), wxPoint(40, 120));
	
    
    
	
	//list box to contain names of input devices
	listboxDevices = new wxListBox(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 20)); 
	
	
	RtAudio audio;
	RtAudio::DeviceInfo info;
	
	//add names
	for (unsigned int i=0; i < audio.getDeviceCount(); i++)
	{
		info = audio.getDeviceInfo( i );
		
		std::string name_and_index = info.name + "[" + std::to_string(i) + "]";
		wxString mystring( name_and_index );
		listboxDevices->Append(mystring);
	}
	
	
	//make horizontal box to put names in
	wxBoxSizer* hboxDeviceNames = new wxBoxSizer(wxHORIZONTAL);
	hboxDeviceNames->Add(listboxDevices, 1, wxEXPAND | wxALL, 20);
    
    //initialize Ok and Cancel buttons 
	startButton = new wxButton(this, wxID_ANY, wxT("Start"), 
							wxDefaultPosition, wxSize(70, 30));
	
	startButton->Bind(wxEVT_BUTTON,&LCCOutputDialog::OnStart,this);
	
	stopButton = new wxButton(this, wxID_ANY, wxT("Stop"), 
							wxDefaultPosition, wxSize(70, 30));
	
	stopButton->Bind(wxEVT_BUTTON,&LCCOutputDialog::OnStop,this);
	
	exitButton = new wxButton(this, wxID_ANY, wxT("Exit"), 
								wxDefaultPosition, wxSize(70, 30));
	
	exitButton->Bind(wxEVT_BUTTON,&LCCOutputDialog::OnExit,this);
	
	//Make vertical box to put horizontal boxes in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
	
	hbox5->Add(startButton,1);
	hbox5->Add(stopButton, 1);
	hbox5->Add(exitButton, 1, wxLEFT, 5);
	
	//add panel of text fields in vertical box
	
	vbox->Add(devicesText);
	vbox->Add(hboxDeviceNames);
	
	wxBoxSizer *hBoxDeviceParam = new wxBoxSizer(wxHORIZONTAL);
	
	hBoxDeviceParam->Add(inputDeviceText);
	hBoxDeviceParam->Add(textField_inputDevice);
	
	hBoxDeviceParam->Add(outputDeviceText);
	hBoxDeviceParam->Add(textField_outputDevice);
	
	vbox->Add(hBoxDeviceParam,1);
	
	
	wxBoxSizer *hBoxSR = new wxBoxSizer(wxHORIZONTAL);
	hBoxSR->Add(sampleRateText);
	hBoxSR->Add(textField_samplerate);
	
	vbox->Add(hBoxSR,1);
		
	wxBoxSizer *hBoxGain = new wxBoxSizer(wxHORIZONTAL);
	
	hBoxGain->Add(inputGainText);
	hBoxGain->Add(textField_inputgain,1, wxLEFT, 5);
	
	hBoxGain->Add(centerGainText);
	hBoxGain->Add(textField_centergain,1, wxLEFT, 5);
	
	hBoxGain->Add(endGainText);
	hBoxGain->Add(textField_endgain,1, wxLEFT, 5);
	
	hBoxGain->Add(decayGainText);
	hBoxGain->Add(textField_decaygain,1, wxLEFT, 5);
	
	vbox->Add(hBoxGain,1);
	
	wxBoxSizer *hBoxDelay = new wxBoxSizer(wxHORIZONTAL);
	
	hBoxDelay->Add(delayUSText);
	hBoxDelay->Add(textField_delay_us);
	
	vbox->Add(hBoxDelay,1);
	
	vbox->Add(hbox5, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 2);

	SetSizerAndFit(vbox);
	
	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy(); 
}


void LCCOutputDialog::ChangeAttributes()
{
	
	
}


void LCCOutputDialog::OnStart(wxCommandEvent& event)
{	
	//get values from text fields
	
	//set to change settings in choice.txt
	
	//changes values in param.txt
	
	//set lcc program to start taking in new input through rw-param-stat.txt
	
}

void LCCOutputDialog::OnStop(wxCommandEvent& event )
{
	//set to quit program in choice.txt
		
	//set lcc program to start taking in new input through rw-param-stat.txt
}

void LCCOutputDialog::OnExit(wxCommandEvent& event)
{
	LCCOutputDialog::Exit();
}

void LCCOutputDialog::Exit()
{
	if(startButton != nullptr){ delete startButton;}
	if(stopButton != nullptr){ delete stopButton;}
	if(exitButton != nullptr){delete exitButton;}

	if(textField_inputDevice != nullptr){ delete textField_inputDevice;}
	if(textField_outputDevice != nullptr){ delete textField_outputDevice;}
	if(textField_inputgain != nullptr){ delete textField_inputgain;}
	if(textField_centergain != nullptr){ delete textField_centergain;}
	if(textField_endgain != nullptr){ delete textField_endgain;}
	if(textField_samplerate != nullptr){ delete textField_samplerate;}
	if(textField_delay_us != nullptr){ delete textField_delay_us;}
	if(textField_decaygain != nullptr){ delete textField_decaygain;}

    if(listboxDevices != nullptr){delete listboxDevices;}
    Close( true ); //close window
}


