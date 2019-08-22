#include "listener-external.h"


ListenerExternal::ListenerExternal(Listener* thisListener)
{
	m_listener_ptr  = thisListener;
	m_ext_orientation_serial_device_ptr = new ExternalOrientationDeviceSerial();
}

void ListenerExternal::SetOrientationByExternalDevice()
{
	if(m_ext_orientation_serial_device_ptr)
	{
		if(m_listener_ptr)
		{
			if(m_listener_ptr->GetListenerExternalDeviceOrientationBool())
			{
				//if device is initialized
				if(m_ext_orientation_serial_device_ptr->GetDeviceInitializedBool())
				{
					float fx,fy,fz,ux,uy,uz;
					
					m_ext_orientation_serial_device_ptr->ReadOrientationParametersFromSerial(&fx,&fy,&fz,&ux,&uy,&uz);
					std::cout << "fx: " << fx << " fy: " << fy << " fz: " << fz << "\n" << \
							" ux: " << ux << " uy: " << uy << " uz: " << uz << std::endl;
								 
					m_listener_ptr->setForwardX(fx);
					m_listener_ptr->setForwardY(fy);
					m_listener_ptr->setForwardZ(fz);
					m_listener_ptr->setUpX(ux);
					m_listener_ptr->setUpY(uy);
					m_listener_ptr->setUpZ(uz);
				}
				
			}
			
		}
		
	}

}

ExternalOrientationDeviceSerial* ListenerExternal::GetExternalOrientationSerialDevicePtr()
{
	return m_ext_orientation_serial_device_ptr;
}
