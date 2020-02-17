#include "armrotation.h"

ArmRotation::ArmRotation (int inputPin) {

  // Set limit and starting values
  maxValue = 1650;
  minValue = 1350;
  stoppedValue=1500;
  currentValue = stoppedValue;
  servo.attach(inputPin); // Associate the motor with the specified pin
  pin = inputPin; // Record the associated pin
  servo.writeMicroseconds(stoppedValue); // Set value to "stopped"
}

int ArmRotation::setValue(int inputValue) {
  // call parent logic (keeps value within preset boundary)
  int value = Output::setValue(inputValue);
  // Actually control the device
  servo.writeMicroseconds(value);
  // Return the set value
  return value;
}

void ArmRotation::turnOff(){
  // Switch off in case of emergency
  currentValue = stoppedValue;
  servo.writeMicroseconds(stoppedValue);
}