#ifndef EXTERNAL_ORIENTATION_DEVICE_SERIAL_H
#define EXTERNAL_ORIENTATION_DEVICE_SERIAL_H

#include <boost/math/quaternion.hpp> //for using quaternion to change orientation
#include "SimpleSerial.h" //for getting serial data
#include <boost/algorithm/string.hpp> //for splitting string

#include <iostream>

#include <wx/wx.h>
#include <wx/timer.h>

class ExternalOrientationDeviceSerial
{

public:
	ExternalOrientationDeviceSerial();
	~ExternalOrientationDeviceSerial();
	
	void InitSerialCommunication(std::string port,unsigned int baud_rate);
	
	void SetDeviceInitializedBool(bool status);
	bool GetDeviceInitializedBool();
	
	void ReadOrientationParametersFromSerial(float* fx,float* fy, float* fz,
											float* ux, float* uy,float* uz);
	
private:
	SimpleSerial* m_serial_ptr;
	bool deviceInitialized;
	
	//quaternions for intial forward vector and up vector directions
	boost::math::quaternion <float> forward_vector_quaternion;
	boost::math::quaternion <float> up_vector_quaternion;

};

//class to use with mainframe
class ExternalDeviceRepeatTimer : public wxTimer
{
public:
    ExternalDeviceRepeatTimer(ExternalOrientationDeviceSerial* device);
    void Notify(); //action to take periodically after certain amount of time defined
    void start();

	
private:
	ExternalOrientationDeviceSerial* m_device;
	Listener* m_listener_ptr;
	
};


#endif
