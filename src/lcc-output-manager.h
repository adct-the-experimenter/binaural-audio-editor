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
	LCCOutputDialog(const wxString& title);


	void OnStart(wxCommandEvent& event );

	void OnExit(wxCommandEvent& event);

	void Exit();


private:
	
	wxButton* startButton;
	wxButton* stopButton;
	wxButton* exitButton;
		
	wxListBox* listboxInputDevices;
	wxListBox* listboxOutputDevices;
	
	int input_selection_index;
	int output_selection_index;
	
	void InputDeviceSelectedInListBox(wxCommandEvent& event );
	void OutputDeviceSelectedInListBox(wxCommandEvent& event );
	
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
	wxTextCtrl* textField_samplerate;
	wxTextCtrl* textField_inputgain;
	wxTextCtrl* textField_centergain;
	wxTextCtrl* textField_endgain;
	wxTextCtrl* textField_decaygain;
	wxTextCtrl* textField_delay_us;
};

#endif
