#include "armgripper.h"

ArmGripper::ArmGripper(int inputPin, int limitPinLeft, int limitPinRight ) {

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