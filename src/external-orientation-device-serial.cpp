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
			
	//read line
	std::string quaternion_data = m_serial_ptr->readLine();

	//parse w,x,y,z data from line read
	boost::split(results, quaternion_data, [](char c){return c == ' ';});

	if(results.size() >= 4)
	{
		//convert number values in string to float 
		float w = std::stof(results[0]);
		float x = std::stof(results[1]);
		float y = std::stof(results[2]);
		float z = std::stof(results[3]);

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
		*fz = rotated_forward_vector_quaternion.R_component_2();
		//if(listenerToManipulatePtr->getForwardZ() != thisForwardZ){listenerToManipulatePtr->setForwardZ(thisForwardZ);}

		*fx = rotated_forward_vector_quaternion.R_component_3();
		//if(listenerToManipulatePtr->getForwardX() != thisForwardX){listenerToManipulatePtr->setForwardX(thisForwardX);}

		*fy = rotated_forward_vector_quaternion.R_component_4();
		//if(listenerToManipulatePtr->getForwardY() != thisForwardY){listenerToManipulatePtr->setForwardY(thisForwardY);}

		*uz = rotated_up_vector_quaternion.R_component_2();
		//if(listenerToManipulatePtr->getUpZ() != thisUpZ){listenerToManipulatePtr->setUpZ(thisUpZ);}

		*ux = rotated_up_vector_quaternion.R_component_3();
		//if(listenerToManipulatePtr->getUpX() != thisUpX){listenerToManipulatePtr->setUpX(thisUpX);}

		*uy = rotated_up_vector_quaternion.R_component_4();
		//if(listenerToManipulatePtr->getUpY() != thisUpY){listenerToManipulatePtr->setUpY(thisUpY);
	}
}

ExternalDeviceRepeatTimer::ExternalDeviceRepeatTimer(ExternalOrientationDeviceSerial* device, Listener* listener) : wxTimer()
{
    m_device = device;
    m_listener_ptr = listener;
}

void ExternalDeviceRepeatTimer::Notify()
{
     
}

void ExternalDeviceRepeatTimer::start()
{
    wxTimer::Start(250,wxTIMER_CONTINUOUS); //the timer calls Notify every 250 milliseconds
}
