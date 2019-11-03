#include "input-devices.h"

Input::Input(){
  pin=0;
  partID="Part ID not set.";
}
int Input::getValue() {
  return 0;
}

int Input::setParam(int index, int value){
  return 0;
}


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
    communication.bufferValue(this->partID+"_Temp",String(temp));

    // Get acceleration data
    imu::Vector<3> euler = imu.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    communication.bufferValue(this->partID+"_AccX",String(euler.x()));
    communication.bufferValue(this->partID+"_AccY",String(euler.y()));
    communication.bufferValue(this->partID+"_AccZ",String(euler.z()));

  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.sendStatus(-3);
    return -1;
  }
  return 0;
}

Depth::Depth(int inputPin, String incomingPartID){
  initialised = false;
  Wire.begin();
  // Run parent method
  partID = incomingPartID;
  if(!depthSensor.init())
  {
    // Send error message
    communication.sendStatus(-4);
  }
  else{
    depthSensor.setModel(MS5837::MS5837_30BA);
    depthSensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
    initialised = true;
  }
}

int Depth::getValue() {
  if(initialised){
    depthSensor.read(); // Read current values
    communication.bufferValue(this->partID+"_Pres",String(depthSensor.pressure()));
    communication.bufferValue(this->partID+"_Temp",String(depthSensor.temperature()));
    communication.bufferValue(this->partID+"_Dep",String(depthSensor.depth()));
    communication.bufferValue(this->partID+"_Alt",String(depthSensor.altitude()));

  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.sendStatus(-5);
    return -1;
  }
  return 0;
}

PHSensor::PHSensor(int inputPin, String incomingPartID){
  partID = incomingPartID;
  pin = inputPin;

}

int PHSensor::getValue() {
  // This might need rethinking since it looks a bit s l o w

  for(int i=0;i<10;i++)       //Get 10 sample values from the sensor to smooth the result
  {
    buf[i]=analogRead(pin);
    delay(1); // This delay might be too short
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++){                      //take the average value of 6 center sample
    avgValue+=buf[i];
  }
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  communication.bufferValue(this->partID,String(phValue)); // Send averaged sensor value
  return 0;
}

Temperature::Temperature(String incomingPartID){
  Wire.begin();
  // Run parent method
  partID = incomingPartID;
  maxAmp.begin(MAX31865_3WIRE);
}

int Temperature::getValue() {
  communication.bufferValue(this->partID,String(maxAmp.temperature(100, 430)));
  // Check and print any faults
  uint8_t fault = maxAmp.readFault();

  if (fault) {
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      communication.sendStatus(-14);
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      communication.sendStatus(-15);
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      communication.sendStatus(-16);
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      communication.sendStatus(-17);
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      communication.sendStatus(-18);
    }
    if (fault & MAX31865_FAULT_OVUV) {
      communication.sendStatus(-19);
    }
    maxAmp.clearFault();
  }
}

Sonar::Sonar(String incomingPartID){
  initialised = false;
  sonStart = 500, sonLen = 30000;
  partID = incomingPartID;
  Serial1.begin(115200); // sonar io
  if(!sonar.initialize())
  {
    // Send error message because sensor not found
    communication.sendStatus(-22);
  }
  else{
    initialised = true;
  }
}

int Sonar::getValue() {
  if(initialised){
    if(sonar.update()){
      communication.bufferValue(this->partID+"_Dist",String(sonar.distance()));
      communication.bufferValue(this->partID+"_Conf",String(sonar.confidence()));
    }
    else{
      // Throw error because this sensor could not update
      communication.sendStatus(-21);
      if(!sonar.initialize())
      {
        // Send error message because sensor not found
        communication.sendStatus(-22);
      }
      else{
        initialised = true;
      }
    }
  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.sendStatus(-20);
  }
  
}

/* Set parameters for sensor */
int Sonar::setParam(int index, int value){
  // Index 1 = start of scanning range
  // Index 2 = length of scanning range
  if(index == 1){
    /* Set the start of the sonar range */
    sonStart = value;
    sonar.set_range(sonStart,sonLen);
  }
  else if(index == 2){
    /* Set the length of the sonar range */
    sonLen = value;
    sonar.set_range(sonStart,sonLen);
  }
  else{
    // Throw error because not valid index
    communication.sendStatus(-23);
  }
}
