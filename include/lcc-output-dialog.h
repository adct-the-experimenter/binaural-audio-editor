#ifndef LCC_DIALOG_H
#define LCC_DIALOG_H

#include <memory>
#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl
#include <wx/listbox.h> //for list box


class LCCOutputDialog : public wxDialog
{

public:
	LCCOutputDialog(const wxString& title, wxWindow* parent);
	~LCCOutputDialog();

	void OnStart(wxCommandEvent& event );
	
	void OnStop(wxCommandEvent& event );
	
	void OnChange(wxCommandEvent& event);
	
	virtual void Close();

	void ExitProgram();

	void ChangeAttributes();
	
	void OnBrowseDataDir(wxCommandEvent& event);
	void OnBrowseExec(wxCommandEvent& event);
	
	void OpenWindow();
	
	void CreateWindow();
	
	void DestroyWindow();
	
	void HideWindow();
	
private:
	
	wxWindow* m_parent_window_ptr;
	
	std::string filePathExec;
	std::string filePathDataDir;
	
	wxButton* startButton;
	wxButton* changeButton;
	wxButton* stopButton;
	wxButton* exitButton;
	
	wxButton* browseDataDirectoryButton;
	wxButton* browseExecButton;
		
	wxListBox* listboxDevices;
	void FillListBoxDevices();
	
	//Inputs
	/*
	 * inputdevice_num} 
	 * {outputdevice_num} 
	 * {sample_rate} 
	 * {inputgain} 
	 * {centergain} 
	 * {endgain} 
	 * {LCC_Decaygain} 
	 * {LCC_Delay_in_microseconds} \n");
	*/
	
	wxTextCtrl* textField_exec;
	wxTextCtrl* textField_dataDir;
	
	wxTextCtrl* textField_inputDevice;
	wxTextCtrl* textField_outputDevice;
	
	wxTextCtrl* textField_samplerate;
	wxTextCtrl* textField_inputgain;
	wxTextCtrl* textField_centergain;
	wxTextCtrl* textField_endgain;
	wxTextCtrl* textField_decaygain;
	wxTextCtrl* textField_delay_us;
	
	wxTextCtrl* textField_output_msg;
	
	void BrowseForInputFilePath(std::string& inputFilePath);
	void BrowseForInputDirectoryPath(std::string& inputDirPath);
	
	bool lccAudioProgramIsRunning;
};

#endif
