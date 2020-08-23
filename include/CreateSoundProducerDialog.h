#ifndef _CREATE_SOUND_PRODUCER_H
#define _CREATE_SOUND_PRODUCER_H

#include <wx/wx.h>

#include <wx/valnum.h> //for wxFloatingPointValidator
#include <wx/textctrl.h> //for wxTextCtrl

#include <wx/checkbox.h>

#include "openalsoftaudioengine.h" //for loading buffer and creating source of sound producer

class CreateSoundProducerDialog : public wxDialog
{

public:
	CreateSoundProducerDialog(const wxString& title, OpenAlSoftAudioEngine* audioEngine);


	void OnOk(wxCommandEvent& event );

	void OnCancel(wxCommandEvent& event);

	void OnBrowse(wxCommandEvent& event);

	void Exit();

	enum
	{
		ID_OK = wxID_HIGHEST + 1,
		ID_CANCEL,
		ID_BROWSE
	};

	//function to return position of new sound producer object
	void getNewPosition(double& x, double& y, double& z);

	std::string getNewName();

	std::string& getSoundFilePath();

	ALuint& getBuffer();

	bool OkClickedOn();
	
	bool getFreeRoamBool();

private:
	wxButton* okButton;
	wxButton* cancelButton;
	wxButton* browseButton;

	wxTextCtrl* textFieldName;

	wxTextCtrl* textFieldX;
	wxTextCtrl* textFieldY;
	wxTextCtrl* textFieldZ;

	wxTextCtrl* textFieldSoundFilePath;
	
	wxCheckBox* checkBoxFreeRoam;
	bool tempFreeRoamBool; 

	OpenAlSoftAudioEngine* ptrAudioEngine;

	std::string name;
	double xPosition;
	double yPosition;
	double zPosition;

	std::string soundFilePath;
	ALuint buffer;
	
	


	bool okClicked; //bool to indicate if ok button was clicked on

	void initPrivateVariables();

	DECLARE_EVENT_TABLE()

};



#endif
