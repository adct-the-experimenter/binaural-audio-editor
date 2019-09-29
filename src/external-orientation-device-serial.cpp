// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifdef WIN32
#include <winsock2.h>
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "external-orientation-device-serial.h"

ExternalOrientationDeviceSerial::ExternalOrientationDeviceSerial()
{
	m_serial_ptr = nullptr;
	deviceInitialized = false;

	forward_vector_quaternion = boost::math::quaternion <float>(0,0,0,-1);
	up_vector_quaternion = boost::math::quaternion <float>(0,0,1,0);
}

ExternalOrientationDeviceSerial::~ExternalOrientationDeviceSerial()
{
	if(m_serial_ptr != nullptr)
	{
		delete m_serial_ptr;
	}
}

void ExternalOrientationDeviceSerial::InitSerialCommunication(std::string port,unsigned int baud_rate)
{
	try
	{
		m_serial_ptr = new SimpleSerial(port,baud_rate);

		if(m_serial_ptr != nullptr)
		{
			ExternalOrientationDeviceSerial::SetDeviceInitializedBool(true);
		}
	}
	catch(boost::system::system_error& e)
	{
		std::cout<< "Error: " << e.what() << std::endl;
		m_serial_ptr = nullptr;
		ExternalOrientationDeviceSerial::SetDeviceInitializedBool(false);
	}
}

void ExternalOrientationDeviceSerial::SetDeviceInitializedBool(bool status){deviceInitialized = status;}

bool ExternalOrientationDeviceSerial::GetDeviceInitializedBool(){return deviceInitialized;}

void ExternalOrientationDeviceSerial::ReadOrientationParametersFromSerial(float* fx,float* fy, float* fz,
											float* ux, float* uy,float* uz)
{
	std::vector<std::string> results;
	std::cout << "In read orientation parameters. \n";
	//read line
	//std::string quaternion_data = m_serial_ptr->readLine();

	//std::cout << "quaternion data: \n" << quaternion_data << std::endl;
	std::string quaternion_data;

	//get rotation quaternion w
	m_serial_ptr->writeString("!"); //send command to display quaternion w
	quaternion_data = m_serial_ptr->readLine(); //read number
	m_serial_ptr->writeString("%"); //send command to stop display
	float w = std::stof(quaternion_data);

	//std::cout << "\n quaternion data w:" << quaternion_data << std::endl;

	//get rotation quaternion x
	m_serial_ptr->writeString("@"); //send command to display quaternion x
	quaternion_data = m_serial_ptr->readLine(); //read number
	m_serial_ptr->writeString("%"); //send command to stop display
	float x = std::stof(quaternion_data);

	//std::cout << "\n quaternion data x:" << quaternion_data << std::endl;

	//get rotation quaternion y
	m_serial_ptr->writeString("#"); //send command to display quaternion y
	quaternion_data = m_serial_ptr->readLine(); //read number
	m_serial_ptr->writeString("%"); //send command to stop display
	float y = std::stof(quaternion_data);

	//std::cout << "\n quaternion data y:" << quaternion_data << std::endl;

	//get rotation quaternion z
	m_serial_ptr->writeString("$"); //send command to display quaternion z
	quaternion_data = m_serial_ptr->readLine(); //read number
	m_serial_ptr->writeString("%"); //send command to stop display
	float z = std::stof(quaternion_data);

	//std::cout << "\n quaternion data z:" << quaternion_data << std::endl;


	boost::math::quaternion <float> rotation_quaternion(w,x,y,z);
	boost::math::quaternion <float> inverse_rotation_quaternion(w,-1*x,-1*y,-1*z);


	//calculate new rotated forward vector
	// P'= R*P*R'
	boost::math::quaternion <float> rotated_forward_vector_quaternion;
	rotated_forward_vector_quaternion = rotation_quaternion * forward_vector_quaternion * inverse_rotation_quaternion;

	boost::math::quaternion <float> rotated_up_vector_quaternion;
	rotated_up_vector_quaternion = rotation_quaternion * up_vector_quaternion * inverse_rotation_quaternion;

	//remap values for binaural audio editor
	//y in binaural audio editor = z in regular cartesian
	//x in binaural audio editor = y in regular cartesian
	//z in binaural audio editor = x in regular cartesian
	*fz = rotated_up_vector_quaternion.R_component_2();

	*fx = rotated_up_vector_quaternion.R_component_4();

	*fy = rotated_up_vector_quaternion.R_component_3();

	*uz = rotated_forward_vector_quaternion.R_component_2();

	*ux = rotated_forward_vector_quaternion.R_component_4();

	*uy = rotated_forward_vector_quaternion.R_component_3();

}

ExternalDeviceRepeatTimer::ExternalDeviceRepeatTimer(ExternalOrientationDeviceSerial* device, Listener* listener) : wxTimer()
{
    m_device = device;
    m_listener_ptr = listener;
    reading_values = false;
}

void ExternalDeviceRepeatTimer::Notify()
{
     if(!ExternalDeviceRepeatTimer::GetReadingValuesBool())
     {
		 ExternalDeviceRepeatTimer::FunctionToRepeat();
	 }
}

void ExternalDeviceRepeatTimer::FunctionToRepeat()
{

}

void ExternalDeviceRepeatTimer::start()
{
    wxTimer::Start(400,wxTIMER_CONTINUOUS); //the timer calls Notify every 250 milliseconds
}

void ExternalDeviceRepeatTimer::SetReadingValuesBool(bool state){reading_values = state;}
bool ExternalDeviceRepeatTimer::GetReadingValuesBool(){return reading_values;}
