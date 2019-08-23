#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int displayVar;

void setup(void) 
{
	Serial.begin(9600);


	/* Initialise the sensor */
	if(!bno.begin())
	{
		/* There was a problem detecting the BNO055 ... check your connections */
		Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
		while(1);
	}

	delay(1000);

	bno.setExtCrystalUse(true);

 displayVar = 0;
}

void loop(void) 
{
	// Request quaternion data from BNO055
	imu::Quaternion quat = bno.getQuat();
  
	HandleSerial(displayVar);
	
	switch(displayVar)
	{
		case 0:{/*Display nothing */ break;}
		case 1:{Serial.println(quat.w(), 4); break;}
		case 2:{Serial.println(quat.x(), 4); break;}
		case 3:{Serial.println(quat.y(), 4); break;}
		case 4:{Serial.println(quat.z(), 4); break;}
	}
	
	// Pause before capturing new data
	delay(100);
}

void HandleSerial(int& var) 
{
  
  
	while (Serial.available() > 0) 
	{
		char incomingCharacter = Serial.read();
		
		switch (incomingCharacter) 
		{
			//send out rotation quaternion w value
			case '!':
			{
				var = 1; 
				break;
			}
			//send out rotation quaternion x value
			case '@':
			{
				var = 2; 
				break;
			}
			//send out rotation quaternion y value
			case '#':
			{
				var = 3;
				break;
			}
			//send out rotation quaternion z value
			case '$':
			{
				var = 4;
				break;
			}
			case '%':
			{
				var = 0;
				break;
			}
		}
	}
}
