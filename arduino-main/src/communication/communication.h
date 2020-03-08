#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Arduino_JSON.h>
#include "../util/constants.h"

#define ELEMENTCOUNT 20

/* ==========================Communication========================== */

/*
  The communication class is the implementation of the method of communicating with the Raspberry Pi.
  External methods can buffer data to be sent to the Pi, and this class will handle conversion to JSON and other formatting.
*/
class Communication{
  private:
    String key[ELEMENTCOUNT];
    String value[ELEMENTCOUNT];
    int currentPosition; // value of next free space
    bool stringComplete = false;  // whether a full JSON string has been received
    String inputString = "";         // a String to hold incoming data
    //String statusKey = "S_";
    //String deviceIdKey = "ID";
    String messageContents = "";
    int currentStatus = 4;
    bool nextValueIsFirstValue = true; // Flag to indicate if this is the first value of the json string so doesn't need a prepended comma

  public:
    Communication();

    /*
      Whether full string had been received
    */
    void setStringComplete(bool complete);

    /*
      Whether full string had been received
    */
    bool stringIsComplete();

    /*
      The latest JSON string read from the serial port
    */
    void setInputString(String inputStr);

    /*
      The latest JSON string read from the serial port
    */
    String getInputString();
  
    /*
      Increment currentValue and send all values if buffer is full
    */
    void incrementPosition();

    /*
      Buffer a key:value pair to be sent to the Pi
    */
    void bufferValue(String device, String incomingValue);

    /*
      Send the current status of this Arduino as its own message (e.g. booting)
    */
    void sendOnlyStatus (int status);

    /*
      Set the current status Arduino as a mid-string key value pair
    */
    void setStatus (int status);

    /*
      Send all buffered values to the Pi
    */
    void sendAll();
};

extern Communication communication; // Object to handle communication between Arduino and Pi

#endif /* COMMUNICATION_H */
