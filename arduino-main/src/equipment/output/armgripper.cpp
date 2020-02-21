#include "armgripper.h"

ArmGripper::ArmGripper(int inputPin, String partID, int limitPinLeft, int limitPinRight ) : EscMotor(inputPin, partID) {
  // Set limit and starting values
  maxValue = 1900;
  minValue = 1100;
  stoppedValue=1500;
  currentValue = stoppedValue;

  // Set limit switches
  leftLimit = limitPinLeft;
  rightLimit = limitPinRight;
  pinMode(limitPinLeft,INPUT_PULLUP);
  pinMode(limitPinRight,INPUT_PULLUP);
}

int ArmGripper::setValue(int inputValue) {//                                                    TODO: in main loop check if hit limit all the time (update: done - please test)

    if(hitLeftLimit() || hitRightLimit()){ // Checks if limit switch pressed and acts accordingly
      return inputValue;
    }
    // call parent logic (keeps value within preset boundary)
    int value = EscMotor::setValue(inputValue);
    // Return the set value
    return value;
}

bool ArmGripper::hitLeftLimit(){ // check if a limit switch was hit
  if(digitalRead(leftLimit)==LOW && currentValue<stoppedValue){ // Low = pressed
    communication.sendStatus(2);
    setValue(stoppedValue);
    return true;
  }
  return false;
}

bool ArmGripper::hitRightLimit(){ // check if a limit switch was hit
  //Serial.println("Pin is");
  //Serial.println(rightLimit);
  if(digitalRead(rightLimit)==LOW && currentValue>stoppedValue){ // Low = pressed
    communication.sendStatus(3);
    setValue(stoppedValue);
    return true;
  }
  return false;
}

void ArmGripper::constantTask(){ // run in main loop: limit checking
  hitLeftLimit(); hitRightLimit();
}

void ArmGripper::turnOff(){
  // Switch off in case of emergency
  setValue(stoppedValue);
}