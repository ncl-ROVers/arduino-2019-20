#include "input.h"

Input::Input(String incomingPartID) : Device(incomingPartID){
  pin=0;
}

int Input::getValue() {
  return 0;
}

int Input::setParam(int index, int value){
  return 0;
}