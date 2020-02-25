#include "mapper.h"

/* Mapper::Mapper(){
    tIDs = {"Thr_FP", "Thr_FS", "Thr_AP", "Thr_AS", "Thr_TFP", "Thr_TFS", "Thr_TAP", "Thr_TAS", "Mot_R", "Mot_G", "Mot_F"};
    iIDs = {"Sen_IMU", "Sen_Dep", "Sen_PH", "Sen_Temp", "Sen_Sonar"};
    mIDs = {"Thr_M"};
} */

void Mapper::mapT(){
    int numberOfThrusters = 8;
    for ( int i = 0; i < numberOfThrusters; i++) {
        tObjects[i] = new Thruster(i, tIDs[i]); // The 8 movement Thrusters
    }
    // Delays between each device so they initialise separately. This helps to give an auditory signal that everything is connected properly.
    delay(2000);
    tObjects[8] = new ArmRotation(10, tIDs[8]); // Rotation motor for the arm
    delay(2000);
    tObjects[9] = new ArmGripper(11, tIDs[9],54,55); // Gripper motor for the arm
    delay(2000);
    tObjects[10] = new ArmGripper(12, tIDs[10],56,57); // Fish box opening
}

void Mapper::mapI(){
    // Map and initialise sensors
    iObjects[0] = new IMU(0,iIDs[0]);
    iObjects[1] = new Depth(0,iIDs[1]);
    iObjects[2] = new PHSensor(56,iIDs[2]);
    iObjects[3] = new Temperature(iIDs[3]);
    iObjects[4] = new Sonar(iIDs[4]);
}

void Mapper::mapM(){
    mObjects[0] = new Thruster(3,mIDs[0]); // Micro ROV Thruster
}

Output* Mapper::getOutput(String jsonID){
    if(arduinoID==ARD + "T"){
    for(int i = 0; i < T_COUNT; i++){
        if(jsonID == tIDs[i]){
            return tObjects[i];
        }
    }
    }
    else if(arduinoID==ARD + "M"){
    for(int i = 0; i < M_COUNT; i++){
        if(jsonID == mIDs[i]){
        return mObjects[i];
        }
    }
    }
    else{
    // Send error message saying the Arduino was not found
    String errorMessage = "getOutput method doesn't have an option for "+arduinoID;
    communication.sendStatus(-6);
    return new Output();
    }
    // Send error message saying the device was not found
    String errorMessage = "Output device ID is not valid: "+jsonID;
    communication.sendStatus(-8);
    return new Output();
}

Input* Mapper::getInput(String jsonID){
    if(arduinoID==ARD + "I"){
    for(int i = 0; i < I_COUNT; i++){
        if(jsonID == iIDs[i]){
        return iObjects[i];
        }
    }
    }
    else{
    // Send error message saying the Arduino was not found
    String errorMessage = "getInput method doesn't have an option for "+arduinoID;
    communication.sendStatus(-7);
    return new Input();
    }
    // Send error message saying the device was not found
    String errorMessage = "Input device ID is not valid: "+jsonID;
    communication.sendStatus(-9);
}

int Mapper::getNumberOfInputs(){
    return I_COUNT;
}

int Mapper::getNumberOfOutputs(){
    if(arduinoID == ARD + "T"){
    return T_COUNT;
    }
    else if(arduinoID == ARD + "M"){
    return M_COUNT;
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
    if(arduinoID == ARD + "T"){
    for(int i = 0; i < T_COUNT; i++){
        tObjects[i]->turnOff();
        delay(125); // delay 125ms between each thruster to avoid sudden power halt
    }
    }
    else if(arduinoID == ARD + "M"){
    for(int i = 0; i < M_COUNT; i++){
        mObjects[i]->turnOff();
    }
    }
    else{
    // Send error message saying the Arduino was not found
    communication.sendStatus(-10);
    }
    communication.sendStatus(1);
}
