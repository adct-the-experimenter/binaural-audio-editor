#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);

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
}

void loop(void) 
{
   
  imu::Quaternion quat = bno.getQuat();           // Request quaternion data from BNO055

  Serial.print(quat.w(), 4);  Serial.print(" "); // Print quaternion w
  Serial.print(quat.x(), 4);  Serial.print(" "); // Print quaternion x
  Serial.print(quat.y(), 4);  Serial.print(" "); // Print quaternion y
  Serial.print(quat.z(), 4);  Serial.print(" "); Serial.println();   // Print quaternion z

  // Pause before capturing new data
  delay(100);
}
