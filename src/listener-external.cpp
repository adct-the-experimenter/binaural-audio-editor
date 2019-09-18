// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <winsock2.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "listener-external.h"


ListenerExternal::ListenerExternal(Listener* thisListener)
{
	m_listener_ptr  = thisListener;

	m_ext_orientation_serial_device_ptr = new ExternalOrientationDeviceSerial();

	m_device_op_repeater = nullptr;
	//m_device_op_repeater = new ExternalDeviceRepeatTimer(m_ext_orientation_serial_device_ptr,m_listener_ptr);

	//m_device_op_repeater->start();
	serialPortPath = "";
}

ListenerExternal::~ListenerExternal()
{
	if(m_ext_orientation_serial_device_ptr != nullptr)
	{
		delete m_ext_orientation_serial_device_ptr;
	}

	if(m_device_op_repeater != nullptr)
	{
		delete m_device_op_repeater;
	}
}

void ListenerExternal::SetOrientationByExternalDevice()
{
	float fx,fy,fz,ux,uy,uz;

	m_ext_orientation_serial_device_ptr->ReadOrientationParametersFromSerial(&fx,&fy,&fz,&ux,&uy,&uz);

	std::cout << "fx:" << fx << ", fy:"  << fy << " ,fz:" << fz << "\n, ux:" << ux << ", uy:" << uy << ", uz:" << uz << std::endl;

	m_listener_ptr->SetWholeOrientation(fx,fy,fz,ux,uy,uz);
}

ExternalOrientationDeviceSerial* ListenerExternal::GetExternalOrientationSerialDevicePtr()
{
	return m_ext_orientation_serial_device_ptr;
}

void ListenerExternal::SetSerialPortPath(std::string port){serialPortPath = port;}
std::string ListenerExternal::GetSerialPortPath(){return serialPortPath;}
