#include "genericEscMotor.h"

EscMotor::EscMotor (int inputPin, String partID) {
  this->partID = partID;
  // Set limit and starting values
  maxValue = 1900;
  minValue = 1100;
  stoppedValue=1500;
  currentValue = stoppedValue;

  pin = inputPin; // Record the associated pin
  motor = Adafruit_PWMServoDriver(); // Call Adafruit constructor
  
  motor.begin();

  //thruster.attach(inputPin); // Associate the thruster with the specified pin
  
  //thruster.writeMicroseconds(stoppedValue); // Set value to "stopped"
}

int EscMotor::setValue(int inputValue) {
  // call parent logic (keeps value within preset boundary)
  int value = Output::setValue(inputValue);
  // Actually control the device
  motor.writeMicroseconds(value);
  // Return the set value
  return value;
}

void EscMotor::turnOff(){
  //Serial.println(partID);
  // Switch off in case of emergency
  currentValue = stoppedValue;
  motor.writeMicroseconds(stoppedValue);
}