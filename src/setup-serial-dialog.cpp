// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <winsock2.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "setup-serial-dialog.h"

SetupSerialDialog::SetupSerialDialog(const wxString & title, ListenerExternal* listenerext)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(500, 250), wxRESIZE_BORDER)
{

	SetupSerialDialog::initPrivateVariables();

	//make horizontal box to put names in
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	//make vertical box to put display info to edit
	wxBoxSizer *vboxEdit = new wxBoxSizer(wxVERTICAL);


	//initialize text field for path to serial port
	textFieldSerialPort = new wxTextCtrl(this,-1, "Serial Port here",wxPoint(95, 20), wxSize(80,20),wxTE_PROCESS_ENTER);
	textFieldSerialPort->Clear();
	wxStaticText* serialPortText = new wxStaticText(this, -1, wxT("Serial Port :"), wxPoint(40, 60));

	//initialize setup button
	setupButton = new wxButton(this, wxID_ANY, wxT("Setup"), wxDefaultPosition, wxSize(70, 30) );
	setupButton->Bind(wxEVT_BUTTON, &SetupSerialDialog::OnSetup,this);

    //add textfield to edit box
	vboxEdit->Add(serialPortText, 1 , wxEXPAND | wxALL, 1);
	vboxEdit->Add(textFieldSerialPort, 1 , wxEXPAND | wxALL, 1);
	vboxEdit->Add(setupButton, 1 , wxEXPAND | wxALL, 1);

	hbox->Add(vboxEdit, 1, wxEXPAND | wxALL, 10);


	//initialize Ok and Cancel buttons
	okButton = new wxButton(this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize(70, 30) );
	okButton->Bind(wxEVT_BUTTON, &SetupSerialDialog::OnOk,this);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize(70, 30) );
	cancelButton->Bind(wxEVT_BUTTON, &SetupSerialDialog::OnCancel,this);



	//make horizontal box to put ok and cancel buttons in
	wxBoxSizer *hboxBottom = new wxBoxSizer(wxHORIZONTAL);

	hboxBottom->Add(okButton, 0);
	hboxBottom->Add(cancelButton, 0, wxLEFT, 5);

	//Make vertical box to put everything in
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	//add panel of text fields in vertical box
	vbox->Add(hbox, 1, wxEXPAND | wxALL, 10);
	vbox->Add(hboxBottom, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizerAndFit(vbox);

	//update position text fields and checkbox to have current properties of listener
	ptrListenerExternal = listenerext;
	if(ptrListenerExternal == nullptr){std::cout << "listener external pointer is nullptr! \n";}
	else
	{

		(*textFieldSerialPort) << ptrListenerExternal->GetSerialPortPath();
	}

	//center and show elements in dialog
	Centre();
	ShowModal();

	//destroy when done showing
	Destroy();
}

void SetupSerialDialog::initPrivateVariables()
{
	ptrListenerExternal = nullptr;
	textFieldSerialPort = nullptr;

	setupButton = nullptr; okButton = nullptr; cancelButton = nullptr;
}

void SetupSerialDialog::ChangeListenerAttributes()
{
	if(ptrListenerExternal != nullptr)
	{
		ptrListenerExternal->SetSerialPortPath(textFieldSerialPort->GetLineText(0).ToStdString());
	}

}

void SetupSerialDialog::OnSetup(wxCommandEvent& event)
{
	if(ptrListenerExternal != nullptr)
	{
		if( textFieldSerialPort->GetLineText(0).ToStdString() != " " )
		{
			if(!ptrListenerExternal->GetExternalOrientationSerialDevicePtr()->GetDeviceInitializedBool())
			{
				ptrListenerExternal->SetSerialPortPath(textFieldSerialPort->GetLineText(0).ToStdString());
				ptrListenerExternal->GetExternalOrientationSerialDevicePtr()->InitSerialCommunication(textFieldSerialPort->GetLineText(0).ToStdString(),9600);

				if(ptrListenerExternal->GetExternalOrientationSerialDevicePtr()->GetDeviceInitializedBool())
				{
					wxMessageBox(wxT("Successfully setup serial communication! \n"));
				}
				else
				{
					//wxString thisString(message);
					wxMessageBox(wxT("Failed to setup serial communication! \n"));
				}

			}
		}

	}
}

void SetupSerialDialog::OnOk(wxCommandEvent& event )
{
	SetupSerialDialog::ChangeListenerAttributes();

	SetupSerialDialog::Exit();
}

void SetupSerialDialog::OnCancel(wxCommandEvent& event)
{
	SetupSerialDialog::Exit();
}

void SetupSerialDialog::Exit()
{
	if(okButton != nullptr){ delete okButton;}
	if(setupButton != nullptr){delete setupButton;}
	if(cancelButton != nullptr){delete cancelButton;}
	if(textFieldSerialPort != nullptr){delete textFieldSerialPort;}
    Close( true ); //close window
}


