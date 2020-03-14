#include "output.h"

Output::Output(String incomingPartID) : Device(incomingPartID){
  maxValue=0;
  minValue=0;
  currentValue=0;
  stoppedValue=0;
  pin=0;
}

int Output::setValue(int inputValue) {
  int value = inputValue;
  // Method to set thrust capped to min and max

  if (value < minValue || value > maxValue) {
    // Send error message saying the incoming value was out of range
    communication.sendStatus(-1);
    return currentValue; // Keep output at same value
  }
  else{
    currentValue = value;
  }
  return value;
  // Then set the value on the device
}

int Output::getValue() {
  return 0;
}

void Output::constantTask(){
}

String Output::getID (){
  return partID;
}

void Output::turnOff(){
}