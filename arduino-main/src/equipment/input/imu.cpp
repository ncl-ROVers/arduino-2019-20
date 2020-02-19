#include "imu.h"

IMU::IMU(int inputPin){
  initialised = false;
  imu = Adafruit_BNO055(55); // IMU device
  if(!imu.begin())
  {
    // Send error message
    communication.setStatus(-2);
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
    communication.bufferValue(event.orientation.x);

    // y
    communication.bufferValue(event.orientation.y);

    // z
    communication.bufferValue(event.orientation.z);

    // Get temperature recorded by IMU
    int8_t temp = imu.getTemp();
    communication.bufferValue(temp);

    // Get acceleration data
    imu::Vector<3> euler = imu.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    communication.bufferValue(euler.x());
    communication.bufferValue(euler.y());
    communication.bufferValue(euler.z());

  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.setStatus(-3);
    return -1;
  }
  return 0;
}
