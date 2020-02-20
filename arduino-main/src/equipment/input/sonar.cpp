#include "sonar.h"

Sonar::Sonar(){
  initialised = false;
  sonStart = 500, sonLen = 30000;
  Serial1.begin(115200); // sonar io
  if(!sonar.initialize())
  {
    // Send error message because sensor not found
    communication.setStatus(-22);
  }
  else{
    initialised = true;
  }
}

int Sonar::getValue() {
  if(initialised){
    if(sonar.update()){
      communication.bufferValue(sonar.distance());
      communication.bufferValue(sonar.confidence());
    }
    else{
      // Throw error because this sensor could not update
      communication.setStatus(-21);
      if(!sonar.initialize())
      {
        // Send error message because sensor not found
        communication.setStatus(-22);
      }
      else{
        initialised = true;
      }
    }
  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.setStatus(-20);
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
    communication.setStatus(-23);
  }
}