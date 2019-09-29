#ifndef LISTENER_EXTERNAL_H
#define LISTENER_EXTERNAL_H

#include "external-orientation-device-serial.h" //for changing listener orientation with an external device
#include "listener.h"


//class to handle controlling listener with external device

class ListenerExternal
{
public:

	ListenerExternal(Listener* thisListener);
	~ListenerExternal();

	ExternalOrientationDeviceSerial* GetExternalOrientationSerialDevicePtr();
	void SetOrientationByExternalDevice();

	void SetSerialPortPath(std::string port);
	std::string GetSerialPortPath();

private:
	ExternalOrientationDeviceSerial* m_ext_orientation_serial_device_ptr;
	Listener* m_listener_ptr;

	ExternalDeviceRepeatTimer* m_device_op_repeater;

	//path to serial port
	std::string serialPortPath;
};

#endif
