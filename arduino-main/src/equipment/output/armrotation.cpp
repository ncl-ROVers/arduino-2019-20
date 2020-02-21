#include "armrotation.h"

ArmRotation::ArmRotation (int inputPin, String partID) : EscMotor(inputPin, partID){
  // Set limit and starting values
  maxValue = 1650;
  minValue = 1350;
  stoppedValue=1500;
  currentValue = stoppedValue;
}

int ArmRotation::setValue(int inputValue) {
  // call parent logic (keeps value within preset boundary)
  int value = EscMotor::setValue(inputValue);
  // Return the set value
  return value;
}

void ArmRotation::turnOff(){
  // Switch off in case of emergency
  setValue(stoppedValue);
}