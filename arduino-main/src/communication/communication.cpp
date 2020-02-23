#include "communication.h"

Communication::Communication(){
  currentPosition = 0;
  // reserve 2000 bytes for the inputString:
  inputString.reserve(200);
}

void Communication::setStringComplete(bool complete){
  stringComplete = complete;
}

bool Communication::stringIsComplete(){
  return stringComplete;
}

void Communication::setInputString(String inputStr){
  inputString = inputStr;
}

String Communication::getInputString(){
  return inputString;
}

void Communication::incrementPosition(){
  currentPosition++;
  if(currentPosition>=ELEMENTCOUNT){
    sendAll();
    currentPosition = 0;
  }
}

void Communication::bufferValue(String device, String incomingValue){
  // buffer a key value pair to be sent with next load
  messageContents+=",\""+device;
  messageContents+="\":\"";
  messageContents+=incomingValue+"\"";
}

void Communication::sendStatus (int status){
  //Hardcoded JSON
  Serial.print("{\"deviceID\":\"");
  Serial.print(arduinoID);
  Serial.print("\",\"status_");
  Serial.print(String(char(EEPROM.read(0))));
  Serial.print("\":\"");
  Serial.print(status);
  Serial.println("\"}");
}

void Communication::sendAll(){
  Serial.print("{\"deviceID\":\"");
  Serial.print(arduinoID);
  Serial.print("\"");
  Serial.print(messageContents);
  Serial.println("}");
  messageContents="";
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
      communication.setStringComplete(true);
      break;
    }
    communication.setInputString(communication.getInputString() + inChar);
  }
}
