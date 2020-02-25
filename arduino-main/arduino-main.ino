/**
 * @file    arduino-main
 *
 * @brief   Embedded software for a marine ROV
 *
 * @author  Antoine Petty, Joe Smith
 *          Adapted from arduino-main-2019 by Antoine Petty, Mark Hazell, Adam Foy, Paweł Czaplewski
 *          
 * Contact: antoine@antoinepetty.co.uk
 *
 */

/* ============================================================ */
/* ======================Import libraries====================== */
/* ============================================================ */
#include <EEPROM.h> // Library for writing to Arduino's non volatile memory
#include <ArduinoJson.h> // JSON encoding and decoding


// Custom ROV Libaries
#include "./src/communication/communication.h"
#include "./src/util/constants.h"
//#include "input-devices.h"
#include "./src/equipment/input/input.h"
//#include "./src/equipment/output/output-devices.h"
#include "./src/equipment/output/output.h"
#include "./src/util/mapper.h"

/* ============================================================ */
/* ==================Set up global variables=================== */
/* ============================================================ */


unsigned long lastMessage; // The timestamp of when the last message was received
bool safetyActive = false; // Whether output devices are stopped because no data was received

Mapper mapper; // Lightweight replacement for a map/dictionary structure to map JSON IDs to objects representing devices.

Communication communication; // Object to allow for communication with the Raspberry Pi over UART



/* ============================================================ */
/* =======================Setup function======================= */
/* =============Runs once when Arduino is turned on============ */
void setup() {
  arduinoID = ARD + String(char(EEPROM.read(0)));

  // initialize serial:
  Serial.begin(115200);
  Serial.println("test");
  communication.sendStatus(4);
  


  // Map inputs and outputs based on which Arduino this is
  if (arduinoID == ARD + "T") {
    mapper.mapT();
  }
  else if (arduinoID == ARD + "I"){
    mapper.mapI();
  }
  else if (arduinoID == ARD + "M"){
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
  if (communication.stringIsComplete()) {

    // Set up JSON parser
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(communication.getInputString());
    // Test if parsing succeeds.
    if (!root.success()) {
      communication.sendStatus(-11);
      prepareForNewMessage();
      return;
    }
    safetyActive = false; // Switch off auto-off because valid message received

    // Act on incoming message accordingly
    if(arduinoID== ARD + "T" || arduinoID == ARD + "M"){
      handleOutputCommands(root);
    }
    else if (arduinoID == ARD + "I"){
      handleSensorCommands(root);
    }
    else{
      communication.sendStatus(-12);
    }
    prepareForNewMessage();

    updateMostRecentMessageTime();

  }

  // Code to run all the time goes here:

  if(arduinoID == ARD + "T" || arduinoID == ARD + "M"){
    // This Arduino is for outputting
    disableOutputsIfNoMessageReceived(safetyShutoffTimeMs);
  }
  else if(arduinoID == ARD + "I"){
    // Output all sensor data
      mapper.sendAllSensors();
  }

}

/* If no valid message has been received within a sensible amount of time, switch all devices off for safety */
void disableOutputsIfNoMessageReceived(int timeInMs){
  if(TimeSinceLastMessageExceeds(timeInMs) && !safetyActive){ // 1 second limit
    safetyActive = true; //activate safety
    communication.sendStatus(-13);
    communication.sendAll();
  }
}

/* Check if it's been a certain amount of time since the last valid message was received */
bool TimeSinceLastMessageExceeds(int timeInMs){
  return millis() - lastMessage > timeInMs;
}

/* Update time last valid message received */
void updateMostRecentMessageTime(){
  lastMessage = millis();
}

/* Handle each control value from the incoming JSON message */
void handleOutputCommands(JsonObject& root){
  for(const auto& current: root){
    // For each incoming value
    int setValue = mapper.getOutput(current.key)->setValue(current.value);
    if(setValue == current.value) {
      communication.sendStatus(0);
    }
  }
}

/* Handle each control value from the incoming JSON message (Ard_I Only) */
void handleSensorCommands(JsonObject& root){
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

/* Send response, clear the input buffer and wait for new incoming message */
void prepareForNewMessage(){
  // Finish by sending all the values
  communication.sendAll();
  // clear the string ready for the next input
  communication.setInputString("");
  communication.setStringComplete(false);
}
