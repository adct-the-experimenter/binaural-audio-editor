#ifndef LISTENER_H
#define LISTENER_H

#include "AL/al.h" //header for OpenAL Soft
#include "AL/alc.h" //header for OpenAL Soft
#include "AL/alext.h" //header for OpenAL Soft

#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

#include <osg/ShapeDrawable> //for object to render on screen
#include <osg/PositionAttitudeTransform> //for matrix transform that moves object rendered
#include <osg/Geode> //for object rendered to be moved on screen by matrix transform



//This is a class that holds positional info on object
//as well as source and buffer components for use with OpenALSoftAudioEngine

//DOES NOT PLAY SOUND. OpenALSoftAudioEngine plays the sound.
//It only holds information to pass to OpenALSoftAudioEngine to play sound.

struct ListenerSaveData
{
	bool freeRoam;
	bool externalOrientation;
};

class Listener
{
public:
	Listener();
	~Listener();

	
	//Listener Position Functions

	void setPositionX(float& x); //set x position of listener
	float getPositionX(); //get x position of listener
	void setPositionY(float& y); //set y position of listener
	float getPositionY(); //get y position of listener
	void setPositionZ(float& z); //set z position of listener
	float getPositionZ(); //get z position of listener
	
	void MoveUp(float& distance);
	void MoveDown(float& distance);
	void MoveForward(float& distance);
	void MoveBack(float& distance);
	void MoveLeft(float& distance);
	void MoveRight(float& distance);
	
	void SetListenerFreeRoamBool(bool thisBool);
	bool GetListenerFreeRoamBool();
	
	//Listener Orientation Functions
	void setForwardX(float& x); //set x of forward of listener
	float getForwardX(); //get x of forward of listener
	void setForwardY(float& y); //set y of forward of listener
	float getForwardY(); //get y of forward of listener
	void setForwardZ(float& z); //set z of forward of listener
	float getForwardZ(); //get z of forward listener

	void setUpX(float& x); //set x of up of listener
	float getUpX(); //get x of up of listener
	void setUpY(float& y); //set y of up of listener
	float getUpY(); //get y of up of listener
	void setUpZ(float& z); //set z of up of listener
	float getUpZ(); //get z of up of listener
	
	void SetWholeOrientation(float& fx, float& fy, float& fz, float& ux, float& uy, float& uz);
	
	void SetListenerExternalDeviceOrientationBool(bool thisBool);
	bool GetListenerExternalDeviceOrientationBool();
	
	osg::ShapeDrawable* getRenderObject();
	
	osg::Geode* getGeodeNode();
	
	osg::PositionAttitudeTransform* getTransformNode();
		
	ListenerSaveData GetListenerSaveData();
	void LoadListenerSaveData(ListenerSaveData& data);
	
private:
	//bool to indicate if the listener position can be changed freely by user or by listener track
	bool freeRoamByUser;
	
	//bool to indicate if the listener orientation can be changed by an external device or by listener track
	bool orientationByExternalDevice;
	
	void initListener();
	
	//position of Listener
	std::vector <float> listener_position_vector;
	enum POSITION_INDEX { X=0,Y=1,Z=2 };
	void setListenerPosition(); //function to set listener position based on listener position vector coordinates
	
	//orientation of Listener
	std::vector<float> listener_orientation_vector; //vector to hold values of listener orientation
	//first 3 values are forward vector xyz , last 3 values are up vector xyz
	//enum to help set orientation vector
	enum ORIENTATION_INDEX { FORWARD_X=0,FORWARD_Y=1,FORWARD_Z=2,
												 UP_X=3, UP_Y=4, UP_Z=5 };
	void setListenerOrientation(); //function to set listener orientation based on listener orientation vector coordinates
	
	//ShapeDrawable object to render
	osg::ref_ptr<osg::ShapeDrawable> m_renderObject;
	
	osg::ref_ptr<osg::Box> m_box; 
	
	//holds geometry information for rendering, moved by transform of matrix
	osg::ref_ptr<osg::Geode> m_geode;
	
	//moves the geode
	osg::ref_ptr<osg::PositionAttitudeTransform> m_paTransform;
	
	ListenerSaveData m_saveData;
};

#endif
