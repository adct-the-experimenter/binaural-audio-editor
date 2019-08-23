# To use IMU BNO055 sensor to control listener orientation.

## Arduino Instructions
  1. Open Arduino IDE and install  Adafruit BNO055 sensor library. https://github.com/adafruit/Adafruit_BNO055
  2. Connect Arduino microcontroller to Adafruit BNO055 breakout board.
  3. Connect USB cable to Arduino microcontroller.
  4. For Linux, run sudo chmod a+rw /dev/ttyACM0 to allow serial port connection to binaural audio editor and Arudino IDE.
  5. In Arduino IDE, open file hardware-code/bno055_abs_orientation/bno055_abs_orientation.ino
  6. Upload the code to the Arduino.
  7. Open binaural-audio-editor
  8. Go to Listener->Setup Serial.
      - For Linux, type /dev/ttyACM0 into serial port text field and click Setup. Click Ok.
  9. Go to Listener->Edit Listener
  10. Check the External Device Orientation box.
  11. The orientation will now change when sound is playing through sound producer track and it will be set by the physical BNO055 IMU sensor.
