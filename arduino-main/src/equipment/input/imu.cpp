#include "imu.h"

IMU::IMU(int inputPin, String incomingPartID){
  initialised = false;
  imu = Adafruit_BNO055(55); // IMU device
  partID = incomingPartID;
  if(!imu.begin())
  {
    // Send error message
    communication.sendStatus(-2);
  }
  else{
    imu.setExtCrystalUse(true);
    initialised = true;
  }
}

int IMU::getValue() {
  if(initialised){
    // Send x, y, z data from this sensor
    /* Get a new sensor event */
    sensors_event_t event;
    imu.getEvent(&event);
    /* Output the floating point data */
    // x
    communication.bufferValue(this->partID+"_X",String(event.orientation.x));

    // y
    communication.bufferValue(this->partID+"_Y",String(event.orientation.y));

    // z
    communication.bufferValue(this->partID+"_Z",String(event.orientation.z));

    // Get temperature recorded by IMU
    int8_t temp = imu.getTemp();
    communication.bufferValue(this->partID+"_T",String(temp));

    // Get acceleration data
    imu::Vector<3> euler = imu.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    communication.bufferValue(this->partID+"_AX",String(euler.x()));
    communication.bufferValue(this->partID+"_AY",String(euler.y()));
    communication.bufferValue(this->partID+"_AZ",String(euler.z()));

  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.sendStatus(-3);
    return -1;
  }
  return 0;
}
