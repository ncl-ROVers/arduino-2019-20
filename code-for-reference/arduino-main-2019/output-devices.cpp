#include "output-devices.h"

Output::Output() {
  maxValue=0;
  minValue=0;
  currentValue=0;
  stoppedValue=0;
  pin=0;
  partID="part ID not set";
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


Thruster::Thruster (int inputPin, String partID) {
  this->partID = partID;
  // Set limit and starting values
  maxValue = 1900;
  minValue = 1100;
  stoppedValue=1500;
  currentValue = stoppedValue;
  thruster.attach(inputPin); // Associate the thruster with the specified pin
  pin = inputPin; // Record the associated pin
  thruster.writeMicroseconds(stoppedValue); // Set value to "stopped"
}

int Thruster::setValue(int inputValue) {
  // call parent logic (keeps value within preset boundary)
  int value = Output::setValue(inputValue);
  // Actually control the device
  thruster.writeMicroseconds(value);
  // Return the set value
  return value;
}

void Thruster::turnOff(){
  //Serial.println(partID);
  // Switch off in case of emergency
  currentValue = stoppedValue;
  thruster.writeMicroseconds(stoppedValue);
}

ArmGripper::ArmGripper(int inputPin, String partID, int limitPinLeft, int limitPinRight ) {
  this->partID = partID;

  // Set limit and starting values
  maxValue = 1900;
  minValue = 1100;
  stoppedValue=1500;
  currentValue = stoppedValue;

  thruster.attach(inputPin); // Associate the motor with the specified pin
  pin = inputPin; // Record the associated pin
  thruster.writeMicroseconds(stoppedValue); // Set value to "stopped"

  // Set limit switches
  leftLimit = limitPinLeft;
  rightLimit = limitPinRight;
  pinMode(limitPinLeft,INPUT_PULLUP);
  pinMode(limitPinRight,INPUT_PULLUP);
}

int ArmGripper::setValue(int inputValue) {//                                                    TODO: in main loop check if hit limit all the time (update: done - please test)

    // call parent logic (keeps value within preset boundary)
    int value = Output::setValue(inputValue); // set currentValue accordingly
    if(hitLeftLimit() || hitRightLimit()){ // Checks if limit switch pressed and acts accordingly
      return currentValue;
    }
    // Actually control the device
    thruster.writeMicroseconds(value);
    // Return the set value
    return value;
}

bool ArmGripper::hitLeftLimit(){ // check if a limit switch was hit
  if(digitalRead(leftLimit)==LOW && currentValue<stoppedValue){ // Low = pressed
    communication.sendStatus(2);
    currentValue = stoppedValue;
    thruster.writeMicroseconds(currentValue);
    return true;
  }
  return false;
}

bool ArmGripper::hitRightLimit(){ // check if a limit switch was hit
  //Serial.println("Pin is");
  //Serial.println(rightLimit);
  if(digitalRead(rightLimit)==LOW && currentValue>stoppedValue){ // Low = pressed
    communication.sendStatus(3);
    currentValue = stoppedValue;
    thruster.writeMicroseconds(currentValue);
    return true;
  }
  return false;
}

void ArmGripper::constantTask(){ // run in main loop: limit checking
  hitLeftLimit(); hitRightLimit();
}

void ArmGripper::turnOff(){
  //Serial.println(partID);
  // Switch off in case of emergency
  currentValue = stoppedValue;
  thruster.writeMicroseconds(stoppedValue);
}


ArmRotation::ArmRotation (int inputPin, String partID) {
  this->partID = partID;

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
