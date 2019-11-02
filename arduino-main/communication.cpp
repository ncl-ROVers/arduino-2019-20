#include "communication.h"
#include "constants.h"

//Communication::currentPosition = 0;

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

void Communication::bufferError(int code){
  // buffer an error message to be sent with next load
  String tempKey = "status_" + String(char(EEPROM.read(0)));
  key[currentPosition] = tempKey;
  value[currentPosition] = code;
  incrementPosition();
}

void Communication::sendStatus (int status){
  // immediately sends current status to pi
  String resString;
  const int capacity = 100;
  StaticJsonBuffer<capacity> jb;
  JsonObject& res = jb.createObject();
  res["deviceID"] = arduinoID; // add Arduino ID to every message
  String tempKey = "status_" + String(char(EEPROM.read(0)));
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
  res["deviceID"] = arduinoID; // add Arduino ID to every message
  for(int i = 0; i < currentPosition; i++){
    // prepare all buffered values
    res[key[i]] = value[i];
  }
  res.printTo(Serial);
  Serial.println();
  currentPosition = 0;
}
