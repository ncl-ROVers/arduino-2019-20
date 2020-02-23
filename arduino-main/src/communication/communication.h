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
    String statusKey = "s";
    String deviceIdKey = "id";
    String messageContents = "";

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
      Send the current status of this Arduino (e.g. booting)
    */
    void sendStatus (int status);

    /*
      Send all buffered values to the Pi
    */
    void sendAll();
};

extern Communication communication; // Object to handle communication between Arduino and Pi

#endif /* COMMUNICATION_H */
