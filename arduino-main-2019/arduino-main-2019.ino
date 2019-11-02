/**
 * @file arduino-main-2019
 *
 * @brief Embedded software for a marine ROV
 *
 * @author Antoine Petty, Mark Hazell, Adam Foy, Paweł Czaplewski
 * Contact: antoine@antoinepetty.co.uk
 *
 */

/* ============================================================ */
/* ======================Import libraries====================== */
/* ============================================================ */
#include <EEPROM.h> // Library for writing to Arduino's non volatile memory
#include <ArduinoJson.h> // JSON encoding and decoding
#include <Servo.h> // For controlling servos and thrusters

// Custom ROV Libaries
#include "communication.h"
#include "constants.h"
#include "input-devices.h"

/* ============================================================ */
/* ==================Set up global variables=================== */
/* ============================================================ */
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether a full JSON string has been received
unsigned long lastMessage; // The timestamp of when the last message was received
bool safetyActive = false; // Whether output devices are stopped because no data was received


/* ============================================================ */
/* =======================Set up classes======================= */
/* ============================================================ */


/* ==========================Abstract========================== */


/*
  An abstract Output which has generic fields for the associated physical pin, min value, max value, stopped value, current value, and JSON ID.
  Also has generic methods for setting a new value (e.g. motor speed) with validation, getting the current value, getting the JSON ID, and switching this device off for safety.
*/
class Output {
    // Designed to be a generic interface for all output devices.

  protected:
    int maxValue=0; // The maximum value accepted to control this device
    int minValue=0; // The minimum value accepted to control this device
    int currentValue=0; // The current value of this device (e.g. PWM value representing speed)
    int stoppedValue=0; // The control value (e.g. PWM value representing speed) which means this device is stopped/not moving
    int pin=0; // The physical pin this is associated with
    String partID="part ID not set"; // The JSON Part ID of this device

  public:
    Output() {
      //Empty constructor to create harmless output
    }

    /*
      Control this device. This method will take e.g. a pwm thruster value representing speed and set the thruster to this value.
    */
    virtual int setValue(int inputValue) {
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

    /*
      Get the current value of this device (EG: Servo position)
    */
    virtual int getValue() {
      return 0;
    }

    /*
      Something which needs to be run all the time
    */
    virtual void constantTask(){
    }

    /*
      Get the JSON ID for this device
    */
    String getID (){
      return partID;
    }

    /*
      Switch device off - for safety
    */
    virtual void turnOff(){
    }
};





/* ===========================Outputs=========================== */

/*
  The Thruster class represents a BlueRobotics T100 or T200 PWM Thruster.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the thruster.
*/
class Thruster: public Output {

  protected:
    // Represents a thruster controlled by an ESC
    Servo thruster; // Using the Servo class because it uses the same values as our ESCs
    const int stoppedValue=1500;

  public:

    Thruster (int inputPin, String partID) {
      this->partID = partID;

      // Set limit and starting values
      maxValue = 1900;
      minValue = 1100;
      currentValue = stoppedValue;
      thruster.attach(inputPin); // Associate the thruster with the specified pin
      pin = inputPin; // Record the associated pin
      thruster.writeMicroseconds(stoppedValue); // Set value to "stopped"
    }


    int setValue(int inputValue) {
      // call parent logic (keeps value within preset boundary)
      int value = Output::setValue(inputValue);
      // Actually control the device
      thruster.writeMicroseconds(value);
      // Return the set value
      return value;
    }

    void turnOff(){
      //Serial.println(partID);
      // Switch off in case of emergency
      currentValue = stoppedValue;
      thruster.writeMicroseconds(stoppedValue);
    }
};

/*
  The ArmGripper class represents a BlueRobotics M100 or M200 PWM motor.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the motor.
  It also has the option of limit switches to stop movement in one direction if pressed.
*/
class ArmGripper: public Output {

  protected:
    // Represents a thruster motor opening and closing gripper
    Servo thruster;
    int leftLimit, rightLimit;
    const int stoppedValue=1500;

 public:

    ArmGripper(int inputPin, String partID, int limitPinLeft, int limitPinRight ) {
      this->partID = partID;

      // Set limit and starting values
      maxValue = 1900;
      minValue = 1100;
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

    int setValue(int inputValue) {//                                                    TODO: in main loop check if hit limit all the time (update: done - please test)

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

    bool hitLeftLimit(){ // check if a limit switch was hit
      if(digitalRead(leftLimit)==LOW && currentValue<stoppedValue){ // Low = pressed
        communication.sendStatus(2);
        currentValue = stoppedValue;
        thruster.writeMicroseconds(currentValue);
        return true;
      }
      return false;
    }
    bool hitRightLimit(){ // check if a limit switch was hit
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

    void constantTask(){ // run in main loop: limit checking
      hitLeftLimit(); hitRightLimit();
    }

    void turnOff(){
      //Serial.println(partID);
      // Switch off in case of emergency
      currentValue = stoppedValue;
      thruster.writeMicroseconds(stoppedValue);
    }
};

/*
  The ArmRotation class represents a BlueRobotics M100 or M200 PWM motor.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the motor.
  This is similar to ArmGripper except it does not have the option for limit switches.
*/
class ArmRotation: public Output {

  protected:
    // Represents a motor controlling arm rotation
    Servo servo;
    const int stoppedValue=1500;

 public:

    ArmRotation (int inputPin, String partID) {
      this->partID = partID;

      // Set limit and starting values
      maxValue = 1650;
      minValue = 1350;
      currentValue = stoppedValue;
      servo.attach(inputPin); // Associate the motor with the specified pin
      pin = inputPin; // Record the associated pin
      servo.writeMicroseconds(stoppedValue); // Set value to "stopped"
    }


    int setValue(int inputValue) {
      // call parent logic (keeps value within preset boundary)
      int value = Output::setValue(inputValue);
      // Actually control the device
      servo.writeMicroseconds(value);
      // Return the set value
      return value;
    }

    void turnOff(){
      // Switch off in case of emergency
      currentValue = stoppedValue;
      servo.writeMicroseconds(stoppedValue);
    }
};

/* ==========================Mapper========================== */
/*
  The Mapper class is a lightweight replacement for a map/dictionary structure.
  Each Arduino ID has its own array of Strings for JSON IDs, which correspond to Input or Output objects to control devices.
*/
class Mapper {
  private:
    // t for Ard_T (Thrusters)
    const static int tCount=11; // Number of devices attached to Arduino T
    Output* tObjects[tCount];  // Devices attached to Arduino T
    String tIDs[tCount] = {"Thr_FP", "Thr_FS", "Thr_AP", "Thr_AS", "Thr_TFP", "Thr_TFS", "Thr_TAP", "Thr_TAS", "Mot_R", "Mot_G", "Mot_F"}; // Device IDs of those attached to Arduino T

    // i for Ard_I (Input)
    const static int iCount=5;
    Input* iObjects[iCount];
    String iIDs[iCount] = {"Sen_IMU", "Sen_Dep", "Sen_PH", "Sen_Temp", "Sen_Sonar"};

    // m for Ard_M (Micro ROV)
    const static int mCount=1; // Number of devices attached to Arduino M
    Output* mObjects[mCount]; // Devices attached to Arduino M
    String mIDs[mCount] = {"Thr_M"}; // Device IDs of those attached to Arduino M


  public:
    /*
      Assign JSON IDs to devices on this Arduino
    */
    void mapT(){
      int numberOfThrusters = 8;
      for ( int i = 0; i < numberOfThrusters; i++) {
        tObjects[i] = new Thruster(2+i, tIDs[i]); // The 8 movement Thrusters
      }
      // Delays between each device so they initialise separately. This helps to give an auditory signal that everything is connected properly.
      delay(2000);
      tObjects[8] = new ArmRotation(10, tIDs[8]); // Rotation motor for the arm
      delay(2000);
      tObjects[9] = new ArmGripper(11, tIDs[9],54,55); // Gripper motor for the arm
      delay(2000);
      tObjects[10] = new ArmGripper(12, tIDs[10],56,57); // Fish box opening
    }

    /*
      Assign JSON IDs to sensors on this Arduino
    */
    void mapI(){
      // Map and initialise sensors
      iObjects[0] = new IMU(0,iIDs[0]);
      iObjects[1] = new Depth(0,iIDs[1]);
      iObjects[2] = new PHSensor(56,iIDs[2]);
      iObjects[3] = new Temperature(iIDs[3]);
      iObjects[4] = new Sonar(iIDs[4]);
    }

    /*
      Assign JSON IDs to devices on this Arduino
    */
    void mapM(){
      mObjects[0] = new Thruster(3,mIDs[0]); // Micro ROV Thruster
    }

    /*
      Get the object representing an output device connected to this Arduino with the specified JSON ID
     */
    Output* getOutput(String jsonID){
      if(arduinoID=="Ard_T"){
        for(int i = 0; i < tCount; i++){
          if(jsonID == tIDs[i]){
            return tObjects[i];
          }
        }
      }
      else if(arduinoID=="Ard_M"){
        for(int i = 0; i < mCount; i++){
          if(jsonID == mIDs[i]){
            return mObjects[i];
          }
        }
      }
      else{
        // Send error message saying the Arduino was not found
        String errorMessage = "getOutput method doesn't have an option for "+arduinoID;
        communication.sendStatus(-6);
        return new Output();
      }
      // Send error message saying the device was not found
      String errorMessage = "Output device ID is not valid: "+jsonID;
      communication.sendStatus(-8);
      return new Output();
    }

    /*
      Get the object representing a senor connected to this Arduino with the specified JSON ID
     */
    Input* getInput(String jsonID){
      if(arduinoID=="Ard_I"){
        for(int i = 0; i < iCount; i++){
          if(jsonID == iIDs[i]){
            return iObjects[i];
          }
        }
      }
      else{
        // Send error message saying the Arduino was not found
        String errorMessage = "getInput method doesn't have an option for "+arduinoID;
        communication.sendStatus(-7);
        return new Input();
      }
      // Send error message saying the device was not found
      String errorMessage = "Input device ID is not valid: "+jsonID;
      communication.sendStatus(-9);
    }

    /*
      Get the number of sensors
     */
    int getNumberOfInputs(){
      return iCount;
    }

    /*
      Get the number of output devices connected to this Arduino
     */
    int getNumberOfOutputs(){
      if(arduinoID == "Ard_T"){
        return tCount;
      }
      else if(arduinoID == "Ard_M"){
        return mCount;
      }
      return 0;
    }

    /*
      Read data from all sensors and send this to the Pi
     */
    void sendAllSensors(){
      int retcode = 0;
      for(int i = 0; i < iCount; i++){
        if (retcode == 0) {
        retcode = iObjects[i]->getValue();
        } else {
          iObjects[i]->getValue();
        }
      }
      if(retcode == 0) {
        communication.sendStatus(0);
      }
      communication.sendAll();
    }

    /*
      Switch off all devices attached to this Arduino.
      This is primarily a safety feature to be used if no control signals are being received.
     */
    void stopOutputs(){
      if(arduinoID == "Ard_T"){
        for(int i = 0; i < tCount; i++){
          tObjects[i]->turnOff();
          delay(125); // delay 125ms between each thruster to avoid sudden power halt
        }
      }
      else if(arduinoID == "Ard_M"){
        for(int i = 0; i < mCount; i++){
          mObjects[i]->turnOff();
        }
      }
      else{
        // Send error message saying the Arduino was not found
        communication.sendStatus(-10);
      }
      communication.sendStatus(1);
    }

};

Mapper mapper; // Lightweight replacement for a map/dictionary structure to map JSON IDs to objects representing devices.

/* ============================================================ */
/* =======================Setup function======================= */
/* =============Runs once when Arduino is turned on============ */
void setup() {
  arduinoID = "Ard_" + String(char(EEPROM.read(0)));

  // initialize serial:
  Serial.begin(9600);
  communication.sendStatus(4);
  // reserve 2000 bytes for the inputString:
  inputString.reserve(200);


  // Map inputs and outputs based on which Arduino this is
  if (arduinoID == "Ard_T") {
    mapper.mapT();
  }
  else if (arduinoID == "Ard_I"){
    mapper.mapI();
  }
  else if (arduinoID == "Ard_M"){
    mapper.mapM();
  }
  communication.sendAll();
  communication.sendStatus(0);
}

/* ============================================================ */
/* =======================Loop function======================== */
/* ======Runs continuously after setup function finishes======= */
void loop() {
  // parse the string when a newline arrives:
  if (stringComplete) {

    // Set up JSON parser
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(inputString);
    // Test if parsing succeeds.
    if (!root.success()) {
      communication.sendStatus(-11);
      communication.sendAll();
      inputString = "";
      stringComplete = false;
      return;
    }
    safetyActive = false; // Switch off auto-off because valid message received

    // Act on incoming message accordingly
    if(arduinoID=="Ard_T" || arduinoID=="Ard_M"){
      for(const auto& current: root){
        // For each incoming value
        int setValue = mapper.getOutput(current.key)->setValue(current.value);
        if(setValue == current.value) {
          communication.sendStatus(0);
        }
      }
    }
    else if (arduinoID=="Ard_I"){
      
      for(const auto& current: root){
        int setValue = current.value;
        
        // Sonar has custom range settings.
        if(current.key == "Sen_Sonar_Start"){
          setValue = mapper.getInput("Sen_Sonar")->setParam(1,current.value);
        }
        else if(current.key == "Sen_Sonar_Len"){
          setValue = mapper.getInput("Sen_Sonar")->setParam(2,current.value);
        }

        if(setValue == current.value) {
          communication.sendStatus(0);
        }
      }
      
    }
    else{
      communication.sendStatus(-12);
    }
    // Finish by sending all the values
    communication.sendAll();
    // clear the string ready for the next input
    inputString = "";
    stringComplete = false;

    // Update time last message received
    lastMessage = millis();

  }

  // Code to run all the time goes here:

  if(arduinoID=="Ard_T" || arduinoID=="Ard_M"){
    // This Arduino is for outputting
    // Check if it's been too long since last message - bad sign
    // Turn everything off
    if(millis() - lastMessage > 1000 && !safetyActive){ // 1 second limit
      safetyActive = true; //activate safety
      communication.sendStatus(-13);
      communication.sendAll();
      mapper.stopOutputs();
    }
  }
  else if(arduinoID=="Ard_I"){
    // Output all sensor data
      mapper.sendAllSensors();
  }

}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;
      break;
    }
    inputString += inChar;
  }
}
