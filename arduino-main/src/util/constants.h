#include <Arduino.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

extern String arduinoID; // JSON ID representing this Arduino (read from long-term memory)

extern String A_O;
extern String A_I;

extern int safetyShutoffTimeMs; // Amount of time in ms before everything gets shut off if no message received

#endif
