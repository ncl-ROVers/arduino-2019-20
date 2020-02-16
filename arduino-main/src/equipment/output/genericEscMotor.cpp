#include "genericEscMotor.h"

/* The following defined constants are based on the servo example provided with the adafruit servo board library */
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

EscMotor::EscMotor (int inputPin, String partID) {
  this->partID = partID;
  // Set limit and starting values
  maxValue = 1900;
  minValue = 1100;
  stoppedValue = 1500;
  currentValue = stoppedValue;

  Serial.print(partID);
  Serial.print("1");

  pin = inputPin; // Record the associated pin
  EscMotor::instantiateMotor();
  Serial.print("2");
  
  Serial.print("3");
  
  Serial.print("4");
  setValue(stoppedValue);
  Serial.println("5");
}

int EscMotor::setValue(int inputValue) {
  Serial.println("ESC Set");
  // call parent logic (keeps value within preset boundary)
  int value = Output::setValue(inputValue);
  // Actually control the device
  Serial.println("Write to motor END OF THE LINE PROBS");
  EscMotor::motor.writeMicroseconds(pin, value);
  // Return the set value
  Serial.println("ESC return");
  return value;
}

void EscMotor::turnOff(){
  //Serial.println("DEBUG: Turning off " + partID);
  // Switch off in case of emergency
  setValue(stoppedValue);
}

bool EscMotor::motorInstantiated = false;

void EscMotor::instantiateMotor(){
  if(!EscMotor::motorInstantiated){
    EscMotor::motor = Adafruit_PWMServoDriver(); // Call Adafruit constructor
    EscMotor::motor.begin();
    // In theory the internal oscillator is 25MHz but it really isn't
    // that precise. You can 'calibrate' by tweaking this number till
    // you get the frequency you're expecting!
    EscMotor::motor.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
    EscMotor::motor.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
    EscMotor::motorInstantiated = true;
  }
}