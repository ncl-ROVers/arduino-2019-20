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


// Custom ROV Libaries
#include "communication.h"
#include "constants.h"
#include "input-devices.h"
#include "output-devices.h"
#include "mapper.h"

/* ============================================================ */
/* ==================Set up global variables=================== */
/* ============================================================ */
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether a full JSON string has been received
unsigned long lastMessage; // The timestamp of when the last message was received
bool safetyActive = false; // Whether output devices are stopped because no data was received


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
