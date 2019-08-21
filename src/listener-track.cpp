#include "listener-track.h"

ListenerTrack::ListenerTrack(const wxString& title) : Track(title)
{
	listenerToManipulatePtr = nullptr;
	
	//initialize tracks
	xTrack = new DoubleTrack("X Track");
	yTrack = new DoubleTrack("Y Track");
	zTrack = new DoubleTrack("Z Track");
	
	//initialize position at origin
	tempX = 0.0; tempY=0.0; tempZ=0.0;
	
	xTrack->SetReferenceToVarToManipulate(&tempX);
	yTrack->SetReferenceToVarToManipulate(&tempY);
	zTrack->SetReferenceToVarToManipulate(&tempZ);
	
	wQuatTrack = new DoubleTrack("X Quat Track");
	xQuatTrack = new DoubleTrack("X Quat Track");
	yQuatTrack = new DoubleTrack("Y Quat Track");
	zQuatTrack = new DoubleTrack("Z Quat Track");
	
	//initialize as no rotation
	tempQuatW = 0.0; tempQuatX = 0.0; tempQuatY = 0.0; tempQuatZ = 0.0;
	
	wQuatTrack->SetReferenceToVarToManipulate(&tempQuatW);
	xQuatTrack->SetReferenceToVarToManipulate(&tempQuatX);
	yQuatTrack->SetReferenceToVarToManipulate(&tempQuatY);
	zQuatTrack->SetReferenceToVarToManipulate(&tempQuatZ);
	
	forward_vector_quaternion = boost::math::quaternion <float>(0,0,0,-1);
	up_vector_quaternion = boost::math::quaternion <float>(0,0,1,0);
	
	Connect(wxEVT_PAINT, wxPaintEventHandler(Track::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(Track::OnSize));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleTrack::OnLeftMouseClick));
	Connect(wxEVT_CONTEXT_MENU, wxCommandEventHandler(DoubleTrack::OnRightMouseClick));
	
}

void ListenerTrack::FunctionToCallInPlayState()
{
	xTrack->FunctionToCallInPlayState();
	yTrack->FunctionToCallInPlayState();
	zTrack->FunctionToCallInPlayState();
	
	wQuatTrack->FunctionToCallInPlayState();
	xQuatTrack->FunctionToCallInPlayState();
	yQuatTrack->FunctionToCallInPlayState();
	zQuatTrack->FunctionToCallInPlayState();
	
	float thisX = float(tempX);
	float thisY = float(tempY);
	float thisZ = float(tempZ);
	
	float thisQuatW = float(tempQuatW);
	float thisQuatX = float(tempQuatX);
	float thisQuatY = float(tempQuatY);
	float thisQuatZ = float(tempQuatZ);
		
	if(listenerToManipulatePtr != nullptr)
	{
		if(!listenerToManipulatePtr->GetListenerFreeRoamBool())
		{
			if(listenerToManipulatePtr->getPositionX() != thisX){listenerToManipulatePtr->setPositionX(thisX);}
		
			if(listenerToManipulatePtr->getPositionY() != thisY){listenerToManipulatePtr->setPositionY(thisY);}
			
			if(listenerToManipulatePtr->getPositionZ() != thisZ){listenerToManipulatePtr->setPositionZ(thisZ);}			
		}
		
		//if listener orientation is not controlled by external device
		if(!listenerToManipulatePtr->GetListenerExternalDeviceOrientationBool())
		{
			//if there is no change in rotation anywhere
			if(thisQuatW == 0 && thisQuatX == 0 && thisQuatY == 0 && thisQuatZ == 0)
			{
				//do nothing
			}
			else
			{
				boost::math::quaternion <float> rotation_quaternion(thisQuatW,thisQuatX,thisQuatY,thisQuatZ); 
				boost::math::quaternion <float> conjugate_rotation_quaternion = conj(rotation_quaternion); 
				
				
				//calculate new rotated forward vector from rotation quaternion
				// P'= R*P*R'
				
				boost::math::quaternion <float> rotated_forward_vector_quaternion; 
				rotated_forward_vector_quaternion = rotation_quaternion * forward_vector_quaternion * conjugate_rotation_quaternion;
				
				
				//calculate new up direction vector from rotation quaternion
				
				boost::math::quaternion <float> rotated_up_vector_quaternion; 
				rotated_up_vector_quaternion = rotation_quaternion * up_vector_quaternion * conjugate_rotation_quaternion;
				
				//remap values for binaural audio editor
				//y in binaural audio editor = z in regular cartesian
				//x in binaural audio editor = y in regular cartesian
				//z in binaural audio editor = x in regular cartesian
				
				//set new forward direction vector
				
				float thisForwardX = rotated_forward_vector_quaternion.R_component_2();
				if(listenerToManipulatePtr->getForwardX() != thisForwardX){listenerToManipulatePtr->setForwardX(thisForwardX);}					
				
				float thisForwardY = rotated_forward_vector_quaternion.R_component_3();
				if(listenerToManipulatePtr->getForwardY() != thisForwardY){listenerToManipulatePtr->setForwardY(thisForwardY);}					
				
				float thisForwardZ = rotated_forward_vector_quaternion.R_component_4();
				if(listenerToManipulatePtr->getForwardZ() != thisForwardZ){listenerToManipulatePtr->setForwardZ(thisForwardZ);}					
				
				//set new up direction vector
				
				float thisUpX = rotated_up_vector_quaternion.R_component_2();
				if(listenerToManipulatePtr->getUpX() != thisUpX){listenerToManipulatePtr->setUpX(thisUpX);}
				
				float thisUpY = rotated_up_vector_quaternion.R_component_3();
				if(listenerToManipulatePtr->getUpY() != thisUpY){listenerToManipulatePtr->setUpY(thisUpY);}
				
				float thisUpZ = rotated_up_vector_quaternion.R_component_4();
				if(listenerToManipulatePtr->getUpZ() != thisUpZ){listenerToManipulatePtr->setUpZ(thisUpZ);}
				
			}
		}
		//else if listener orientation is controlled by external device
		else
		{
			//std::cout << "listener orientation by external device called! \n";
			listenerToManipulatePtr->SetOrientationByExternalDevice();
		}
			
			
	}
}

void ListenerTrack::FunctionToCallInPauseState(){}
void ListenerTrack::FunctionToCallInRewindState(){}
void ListenerTrack::FunctionToCallInFastForwardState(){}
void ListenerTrack::FunctionToCallInNullState(){}

void ListenerTrack::SetReferenceToListenerToManipulate(Listener* thisListener){listenerToManipulatePtr = thisListener;}

DoubleTrack* ListenerTrack::GetReferenceToXTrack(){return xTrack;}
DoubleTrack* ListenerTrack::GetReferenceToYTrack(){return yTrack;}
DoubleTrack* ListenerTrack::GetReferenceToZTrack(){return zTrack;}

DoubleTrack* ListenerTrack::GetReferenceToQuatWTrack(){return wQuatTrack;}
DoubleTrack* ListenerTrack::GetReferenceToQuatXTrack(){return xQuatTrack;}
DoubleTrack* ListenerTrack::GetReferenceToQuatYTrack(){return yQuatTrack;}
DoubleTrack* ListenerTrack::GetReferenceToQuatZTrack(){return zQuatTrack;}

void ListenerTrack::InitTrack(wxWindow* parent, std::vector <int> *timeTickVector)
{
		
}	

void ListenerTrack::SetupAxisForPositionVariable(double& start, double& end,double& resolution, int& numTick)
{
	xTrack->SetupAxisForVariable(start,end,resolution,numTick);
	yTrack->SetupAxisForVariable(start,end,resolution,numTick);
	zTrack->SetupAxisForVariable(start,end,resolution,numTick);
	
}

void ListenerTrack::SetupAxisForOrientationVariable(double& start, double& end,double& resolution, int& numTick)
{
	wQuatTrack->SetupAxisForVariable(start,end,resolution,numTick);
	xQuatTrack->SetupAxisForVariable(start,end,resolution,numTick);
	yQuatTrack->SetupAxisForVariable(start,end,resolution,numTick);
	zQuatTrack->SetupAxisForVariable(start,end,resolution,numTick);
	
}

void ListenerTrack::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	
	xTrack->render(dc);
	yTrack->render(dc);
	zTrack->render(dc);
	
	wQuatTrack->render(dc);
	xQuatTrack->render(dc);
	yQuatTrack->render(dc);
	zQuatTrack->render(dc);
	
}

void ListenerTrack::OnScroll(wxScrollEvent& event)
{
	Refresh();
	
	FitInside();
	
}

void ListenerTrack::OnSize(wxSizeEvent& event)
{
	Refresh();
	
	FitInside();
	
}
		
void ListenerTrack::SetReferenceToCurrentTimeVariable(double* thisTimeVariable)
{
	Track::SetReferenceToCurrentTimeVariable(thisTimeVariable);
	
	xTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	yTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	zTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	
	wQuatTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	xQuatTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	yQuatTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
	zQuatTrack->SetReferenceToCurrentTimeVariable(thisTimeVariable);
}

void ListenerTrack::SetReferenceToTimeTickVector(std::vector <int> *thisVector)
{
	Track::SetReferenceToTimeTickVector(thisVector);
	xTrack->SetReferenceToTimeTickVector(thisVector);
	yTrack->SetReferenceToTimeTickVector(thisVector);
	zTrack->SetReferenceToTimeTickVector(thisVector);
	
	wQuatTrack->SetReferenceToTimeTickVector(thisVector);
	xQuatTrack->SetReferenceToTimeTickVector(thisVector);
	yQuatTrack->SetReferenceToTimeTickVector(thisVector);
	zQuatTrack->SetReferenceToTimeTickVector(thisVector);
}

std::vector <int> *ListenerTrack::GetReferenceToTimeTickVector(){return Track::GetReferenceToTimeTickVector();}

double ListenerTrack::GetCurrentTime(){return Track::GetCurrentTime();}
		
void ListenerTrack::OnLeftMouseClick(wxMouseEvent& event)
{
	xTrack->logic_left_click();
	yTrack->logic_left_click();
	zTrack->logic_left_click();
	
	wQuatTrack->logic_left_click();
	xQuatTrack->logic_left_click();
	yQuatTrack->logic_left_click();
	zQuatTrack->logic_left_click();
	
	event.Skip();
}

void ListenerTrack::OnRightMouseClick(wxCommandEvent& event)
{
	xTrack->logic_right_click();
	yTrack->logic_right_click();
	zTrack->logic_right_click();
	
	wQuatTrack->logic_right_click();
	xQuatTrack->logic_right_click();
	yQuatTrack->logic_right_click();
	zQuatTrack->logic_right_click();
	
	event.Skip();
}

