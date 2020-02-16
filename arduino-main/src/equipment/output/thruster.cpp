#include "thruster.h"

Thruster::Thruster (int inputPin, String partID) : EscMotor(inputPin, partID){
  // Set limit and starting values
  maxValue = 1900;
  minValue = 1100;
  stoppedValue=1500;
  currentValue = stoppedValue;
}

int Thruster::setValue(int inputValue) {
  // call parent logic (keeps value within preset boundary)
  int value = EscMotor::setValue(inputValue);
  // Return the set value
  return value;
}

void Thruster::turnOff(){
  // Switch off in case of emergency
  Serial.println("Turning off " + partID);
  //EscMotor::setValue(stoppedValue);
  setValue(stoppedValue);
}