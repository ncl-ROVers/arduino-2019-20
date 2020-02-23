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
  key[currentPosition] = device;
  value[currentPosition] = incomingValue;
  incrementPosition();
}

void Communication::sendStatus (int status){
  // immediately sends current status to pi
  String resString;
  const int capacity = 100;
  StaticJsonBuffer<capacity> jb;
  JsonObject& res = jb.createObject();
  res[deviceIdKey] = arduinoID; // add Arduino ID to every message
  String tempKey = statusKey + String(char(EEPROM.read(0)));
  res[tempKey] = status;
  res.printTo(Serial);
  Serial.println();
}

void Communication::sendAll(){
  //      if(currentPosition == 0) {
  //        return;
  //      }
  String resString;
  const int capacity = 1000; // Not sure about this size - probably needs calculating
  StaticJsonBuffer<capacity> jb;
  JsonObject& res = jb.createObject();
  res[deviceIdKey] = arduinoID; // add Arduino ID to every message
  for(int i = 0; i < currentPosition; i++){
    // prepare all buffered values
    res[key[i]] = value[i];
  }
  res.printTo(Serial);
  Serial.println();
  currentPosition = 0;
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
