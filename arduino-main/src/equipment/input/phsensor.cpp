#include "phsensor.h"

PHSensor::PHSensor(int inputPin, String incomingPartID) : Input(incomingPartID){
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