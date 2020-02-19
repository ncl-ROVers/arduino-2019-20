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
}

void Communication::bufferValue(int incomingValue){
  // buffer a key value pair to be sent with next load
  value[currentPosition] = incomingValue;
  incrementPosition();
}

void Communication::bufferError(int code){
  // buffer an error message to be sent with next load
  value[currentPosition] = code;
  incrementPosition();
}

void Communication::setStatus (int incomingStatus){
  status = incomingStatus;
}

void Communication::sendAll(){
  // First 2 chars are Arduino ID
  // For debugging purposes this will be 01
  String outgoingMessage = "01";

  // 2nd set of bytes is status
  outgoingMessage = outgoingMessage + formatInt(status, 4);

  // TODO: For number of output messages, append 4 char values to the outgoing message then send



// old code
  //      if(currentPosition == 0) {
  //        return;
  //      }
  /*
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
  */
}

String Communication::formatInt(int integerToConvert, int numberOfChars){
  int currentSize = String(integerToConvert).length();
  // TODO: Make it so this int is formatted into the number of chars specified

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
