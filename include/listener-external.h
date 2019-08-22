#ifndef LISTENER_EXTERNAL_H
#define LISTENER_EXTERNAL_H

#include "listener.h"
#include "external-orientation-device-serial.h" //for changing listener orientation with an external device

//class to handle controlling listener with external device

class ListenerExternal
{
public:

	ListenerExternal(Listener* thisListener);
	~ListenerExternal();
	
	ExternalOrientationDeviceSerial* GetExternalOrientationSerialDevicePtr();
	void SetOrientationByExternalDevice();
	
private:
	ExternalOrientationDeviceSerial* m_ext_orientation_serial_device_ptr;
	Listener* m_listener_ptr;
		
	//path to serial port
	std::string serialPortPath;
};

#endif
