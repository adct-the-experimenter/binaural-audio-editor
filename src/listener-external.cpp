#include "listener-external.h"


ListenerExternal::ListenerExternal(Listener* thisListener)
{
	m_listener_ptr  = thisListener;
	
	m_ext_orientation_serial_device_ptr = new ExternalOrientationDeviceSerial();
	
	m_device_op_repeater = new ExternalDeviceRepeatTimer(m_ext_orientation_serial_device_ptr,m_listener_ptr);
	
	m_device_op_repeater->start();
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
	
}

ExternalOrientationDeviceSerial* ListenerExternal::GetExternalOrientationSerialDevicePtr()
{
	return m_ext_orientation_serial_device_ptr;
}

void ListenerExternal::SetSerialPortPath(std::string port){serialPortPath = port;}
std::string ListenerExternal::GetSerialPortPath(){return serialPortPath;}
