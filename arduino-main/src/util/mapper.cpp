#include "mapper.h"

/* Mapper::Mapper(){
    tIDs = {"Thr_FP", "Thr_FS", "Thr_AP", "Thr_AS", "Thr_TFP", "Thr_TFS", "Thr_TAP", "Thr_TAS", "Mot_R", "Mot_G", "Mot_F"};
    iIDs = {"Sen_IMU", "Sen_Dep", "Sen_PH", "Sen_Temp", "Sen_Sonar"};
    mIDs = {"Thr_M"};
} */

void Mapper::mapO(){
    int numberOfThrusters = 8;
    for ( int i = 0; i < numberOfThrusters; i++) {
        oObjects[i] = new Thruster(i, oIDs[i]); // The 8 movement Thrusters
    }
    // Delays between each device so they initialise separately. This helps to give an auditory signal that everything is connected properly.
    delay(2000);
    oObjects[8] = new ArmGripper(8, oIDs[8],54,55); // Gripper motor for the arm
    delay(2000);
    oObjects[9] = new Thruster(9, oIDs[9]); // The 8 movement Thrusters
    delay(2000);
    oObjects[10] = new ArmRotation(10, oIDs[10]); // Micro ROV return cord - TODO: Make new class for this
}

void Mapper::mapI(){
    // Map and initialise sensors
    iObjects[0] = new IMU(0, iIDs[0]);
    iObjects[1] = new Depth(0, iIDs[1]);
}

void Mapper::instantiateMap(){
    if(thisIsArduino("O")){
        mapO();
    }
    else if(thisIsArduino("I")){
        mapI();
    }
    else{
        communication.sendStatus(-12);
    }
}

Output* Mapper::getOutput(String jsonID){
    if(thisIsArduino("O")){
        for(int i = 0; i < O_COUNT; i++){
            if(jsonID == oIDs[i]){
            return oObjects[i];
            }
        }
        // Send error message saying the device was not found
        communication.sendStatus(-8);
        return new Output();
    }
    else{
        // Send error message saying the Arduino was not found
        String errorMessage = "getOutput method doesn't have an option for "+arduinoID;
        communication.sendStatus(-6);
        return new Output();
    }
    
}

Input* Mapper::getInput(String jsonID){
    if(thisIsArduino("I")){
        for(int i = 0; i < I_COUNT; i++){
            if(jsonID == iIDs[i]){
            return iObjects[i];
            }
        }
        // Send error message saying the device was not found
        String errorMessage = "Input device ID is not valid: "+jsonID;
        communication.sendStatus(-9);
    }
    else{
        // Send error message saying the Arduino was not found
        String errorMessage = "getInput method doesn't have an option for "+arduinoID;
        communication.sendStatus(-7);
        return new Input();
    }
    
}

int Mapper::getNumberOfInputs(){
    if(thisIsArduino("I")){
        return I_COUNT;
    }
    return 0;
}

int Mapper::getNumberOfOutputs(){
    if(thisIsArduino("O")){
        return O_COUNT;
    }
    return 0;
}

void Mapper::sendAllSensors(){
    int retcode = 0;
    for(int i = 0; i < I_COUNT; i++){
        if (retcode == 0) {
            retcode = iObjects[i]->getValue();
        } else {
            iObjects[i]->getValue();
        }
    }
    if(retcode == 0) {
        communication.sendStatus(0);
    }
    communication.sendAll();
}

void Mapper::stopOutputs(){
    if(thisIsArduino("O")){
        for(int i = 0; i < O_COUNT; i++){
            oObjects[i]->turnOff();
            delay(125); // delay 125ms between each thruster to avoid sudden power halt
        }
    }
    else{
        // Send error message saying the Arduino was not found
        communication.sendStatus(-10);
    }
    communication.sendStatus(1);
}

bool Mapper::thisIsArduino(String arduinoIdToCheck){
    return arduinoID == ARD + arduinoIdToCheck;    
}

bool Mapper::thisIsAnOutputArduino(){
    return thisIsArduino("O");
}

bool Mapper::thisIsAnInputArduino(){
    return thisIsArduino("I");
}