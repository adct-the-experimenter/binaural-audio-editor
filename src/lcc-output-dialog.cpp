#include "lcc-output-dialog.h"

#include <stdio.h>
#include <iostream>
#include <wx/string.h>
#include <fstream>

LCCOutputDialog::LCCOutputDialog(const wxString& title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{	

	//initialize text fields
	
	wxIntegerValidator<unsigned long> validatorInt(NULL, wxNUM_VAL_THOUSANDS_SEPARATOR);
	validatorInt.SetRange(0,20);
	
	
	wxFloatingPointValidator <double> validatorFloat(3,nullptr,wxNUM_VAL_ZERO_AS_BLANK);
    validatorFloat.SetRange(0.00,10.00);     // set allowable range
    
    
    validatorFloat.SetRange(-100.00,100.00);     // set allowable range
    
    textField_exec = new wxTextCtrl(this,-1, " executable", 
								wxPoint(95, 20), wxSize(300,20),
								wxTE_PROCESS_ENTER);
								
	textField_dataDir = new wxTextCtrl(this,-1, " data", 
								wxPoint(95, 20), wxSize(300,20),
								wxTE_PROCESS_ENTER);
	
	textField_samplerate = new wxTextCtrl(this,-1, "44000", 
								wxPoint(95, 60), wxSize(80,20),
								wxTE_PROCESS_ENTER);
	
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
	
	textField_endgain = new wxTextCtrl(this,-1, "2.00", 
								wxPoint(95, 100), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorFloat,          // associate the text box with the desired validator
								wxT("")); 
								
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
	
	textField_inputDevice = new wxTextCtrl(this,-1, "5", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorInt,          // associate the text box with the desired validator
								wxT(""));
	
	textField_outputDevice = new wxTextCtrl(this,-1, "2", 
								wxPoint(95, 20), wxSize(80,20),
								wxTE_PROCESS_ENTER,
								validatorInt,          // associate the text box with the desired validator
								wxT(""));
	
	//initialize text to the left of text fields
    wxStaticText* execFPText = new wxStaticText(this, -1, wxT("Executable"), wxPoint(40, 120));
    wxStaticText* dataDirFPText = new wxStaticText(this, -1, wxT("Data Directory"), wxPoint(40, 120));
    //wxStaticText* devicesText = new wxStaticText(this, -1, wxT("Devices"), wxPoint(40, 120));
    wxStaticText* inputDeviceText = new wxStaticText(this, -1, wxT("Input Device:"), wxPoint(40, 120));
    wxStaticText* outputDeviceText = new wxStaticText(this, -1, wxT("Output Device:"), wxPoint(40, 120));
    wxStaticText* sampleRateText = new wxStaticText(this, -1, wxT("Sample Rate:"), wxPoint(40, 120));
	wxStaticText* inputGainText = new wxStaticText(this, -1, wxT("Input Gain:"), wxPoint(40, 120));
	wxStaticText* centerGainText = new wxStaticText(this, -1, wxT("Center Gain:"), wxPoint(40, 120));
	wxStaticText* endGainText = new wxStaticText(this, -1, wxT("End Gain:"), wxPoint(40, 120));
	wxStaticText* decayGainText = new wxStaticText(this, -1, wxT("Decay Gain:"), wxPoint(40, 120));
	wxStaticText* delayUSText = new wxStaticText(this, -1, wxT("Delay (microseconds):"), wxPoint(40, 120));
	
    
	
	//list box to contain names of input devices
	//listboxDevices = new wxListBox(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 20)); 
	
	/*
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
	*/
	
	//make horizontal box to put names in
	//wxBoxSizer* hboxDeviceNames = new wxBoxSizer(wxHORIZONTAL);
	//hboxDeviceNames->Add(listboxDevices, 1, wxEXPAND | wxALL, 20);
    
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
	
	browseDataDirectoryButton = new wxButton(this, wxID_ANY, wxT("Browse Data"), 
							wxDefaultPosition, wxSize(70, 30));
							
	browseDataDirectoryButton->Bind(wxEVT_BUTTON,&LCCOutputDialog::OnBrowseDataDir,this);
							
	browseExecButton = new wxButton(this, wxID_ANY, wxT("Browse Exec"), 
							wxDefaultPosition, wxSize(70, 30));
							
	browseExecButton->Bind(wxEVT_BUTTON,&LCCOutputDialog::OnBrowseExec,this);
	
	//Make vertical box to put horizontal boxes in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	
	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
	
	hbox5->Add(startButton,1);
	hbox5->Add(stopButton, 1);
	hbox5->Add(exitButton, 1, wxLEFT, 5);
	
	//add panel of text fields in vertical box
	
	wxBoxSizer *hBoxExecutable = new wxBoxSizer(wxHORIZONTAL);
	hBoxExecutable->Add(execFPText);
	hBoxExecutable->Add(textField_exec,1);
	hBoxExecutable->Add(browseExecButton,1);
	
	vbox->Add(hBoxExecutable,1);
	
	wxBoxSizer *hBoxDataDir = new wxBoxSizer(wxHORIZONTAL);
	hBoxDataDir->Add(dataDirFPText);
	hBoxDataDir->Add(textField_dataDir,1);
	hBoxDataDir->Add(browseDataDirectoryButton,1);
	
	vbox->Add(hBoxDataDir,1);
	
	//vbox->Add(devicesText);
	//vbox->Add(hboxDeviceNames);
	
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
	
	std::string inputDeviceStr = std::string(textField_inputDevice->GetLineText(0).mb_str());
	std::string outputDeviceStr = std::string(textField_outputDevice->GetLineText(0).mb_str());
	std::string sampleRateStr = std::string(textField_samplerate->GetLineText(0).mb_str());
	std::string inputgainStr = std::string(textField_inputgain->GetLineText(0).mb_str());
	std::string centergainStr = std::string(textField_centergain->GetLineText(0).mb_str());
	std::string endgainStr = std::string(textField_endgain->GetLineText(0).mb_str());
	std::string decaygainStr = std::string(textField_decaygain->GetLineText(0).mb_str());
	std::string delay_usStr = std::string(textField_delay_us->GetLineText(0).mb_str());
	
	std::string command_one = filePathExec + " " + inputDeviceStr + " " + outputDeviceStr + " "
							  + sampleRateStr + " " + inputgainStr + " " + 
							  centergainStr + " " + endgainStr + " " + 
							  decaygainStr + " " + delay_usStr;
	
	std::cout << command_one << std::endl;
	
	char write = 'w';
	popen(command_one.c_str(),&write);
	
	//set to change settings in choice.txt
	std::ofstream choice_out;
	std::string choice_fpStr = filePathDataDir + "/choice.txt";
	choice_out.open(choice_fpStr.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(choice_out.is_open())
	{
		choice_out << "3";
	}
	
	//changes values in param.txt
	std::ofstream param_out;
	std::string param_fpStr = filePathDataDir + "/param.txt";
	param_out.open(param_fpStr.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(param_out.is_open())
	{
		param_out << inputDeviceStr << "\n";
		param_out << outputDeviceStr << "\n";
		param_out << sampleRateStr << "\n";
		param_out << inputgainStr << "\n";
		param_out << centergainStr << "\n";
		param_out << endgainStr << "\n";
		param_out << decaygainStr << "\n";
		param_out << delay_usStr << "\n";
	}
	
	//set lcc program to start taking in new input through rw-param-stat.txt
	std::ofstream rw_param_stat_out;
	std::string param_status_fp = filePathDataDir + "/rw-param-status.txt";
	rw_param_stat_out.open (param_status_fp.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(rw_param_stat_out.is_open())
	{
		rw_param_stat_out << "1";
	}
}

void LCCOutputDialog::OnStop(wxCommandEvent& event )
{
	//set to quit program in choice.txt
	std::ofstream choice_out;
	std::string choice_fpStr = filePathDataDir + "/choice.txt";
	choice_out.open(choice_fpStr.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(choice_out.is_open())
	{
		choice_out << "4";
	}
	
	//set lcc program to start taking in new input through rw-param-stat.txt
	std::ofstream rw_param_stat_out;
	std::string param_status_fp = filePathDataDir + "/rw-param-status.txt";
	rw_param_stat_out.open (param_status_fp.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(rw_param_stat_out.is_open())
	{
		rw_param_stat_out << "1";
	}
}

void LCCOutputDialog::OnExit(wxCommandEvent& event)
{
	LCCOutputDialog::Exit();
}

void LCCOutputDialog::OnBrowseDataDir(wxCommandEvent& event)
{
	std::string path = "";
	LCCOutputDialog::BrowseForInputDirectoryPath(path);
	
	if(path != "")
	{
		filePathDataDir = path;
	}
	
	if(textField_dataDir)
	{
		textField_dataDir->Clear();
		(*textField_dataDir) << filePathDataDir;
	}
	
}

void LCCOutputDialog::OnBrowseExec(wxCommandEvent& event)
{
	std::string path = "";
	LCCOutputDialog::BrowseForInputFilePath(path);
	
	if(path != "")
	{
		filePathExec = path;
	}
	
	
	if(textField_exec)
	{
		textField_exec->Clear();
		(*textField_exec) << filePathExec;
	}
	
}

void LCCOutputDialog::Exit()
{
	if(startButton != nullptr){ delete startButton;}
	if(stopButton != nullptr){ delete stopButton;}
	if(exitButton != nullptr){delete exitButton;}
	if(browseDataDirectoryButton != nullptr){delete browseDataDirectoryButton;}
	if(browseExecButton != nullptr){delete browseExecButton;}

	if(textField_inputDevice != nullptr){ delete textField_inputDevice;}
	if(textField_outputDevice != nullptr){ delete textField_outputDevice;}
	if(textField_inputgain != nullptr){ delete textField_inputgain;}
	if(textField_centergain != nullptr){ delete textField_centergain;}
	if(textField_endgain != nullptr){ delete textField_endgain;}
	if(textField_samplerate != nullptr){ delete textField_samplerate;}
	if(textField_delay_us != nullptr){ delete textField_delay_us;}
	if(textField_decaygain != nullptr){ delete textField_decaygain;}

    //if(listboxDevices != nullptr){delete listboxDevices;}
    Close( true ); //close window
}


void LCCOutputDialog::BrowseForInputFilePath(std::string& inputFilePath)
{
	wxFileDialog fileDlg(this, _("Choose file"), wxEmptyString, wxEmptyString, _(""));
	if (fileDlg.ShowModal() == wxID_OK)
	{
		
		wxString path = fileDlg.GetPath();
		//use this path in your app
		inputFilePath = std::string(path.mb_str());
	} 
}

void LCCOutputDialog::BrowseForInputDirectoryPath(std::string& inputDirPath)
{
	wxDirDialog dirDlg(NULL, "Choose input directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	
	if (dirDlg.ShowModal() == wxID_OK)
	{
		
		wxString path = dirDlg.GetPath();
		//use this path in your app
		inputDirPath = std::string(path.mb_str());
	}
	
}
