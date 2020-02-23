# Arduino

Low-level software for ROV. Takes JSON key:value pairs from [Raspberry Pi](https://github.com/ncl-ROVers/raspberry-pi) and acts on them accordingly.

## Installation

To install this code on an Arduino you first need to run the appropriate setup script (in arduino-setup ) to assign an ID to the Arduino. This ID dictates the functionality as follows:

* Ard_T: Control for Thrusters on main ROV body
* Ard_M: Control for any Micro ROV devices
* Ard_I: Sending sensor data back up to surface

After running the setup script, flash arduino-main to the device.

### Dependencies

You need to install the following libraries in your IDE: 

- adafruit bno055
- adafruit circuit playground
- adafruit max31865
- arduinojson (select version 5.13.4)
- bluerobotics ms5837
- servo
- Blue Robotics ping-arduino
- adafruit unified sensor

## Expected behaviour

### Arduino O

This Arduino is for controlling the output devices on the main ROV body.

Values in the range 1100 to 1900 will be accepted for Thruster or Motor control where 1100 is full reverse, 1500 is stopped, and 1900 is full forward. However, it's not advised to use values lower than 1350 or higher than 1650 for arm rotation or fish box due to the gearing on these.

Thrusters are given an ID which describes their position on the ROV. Motors are named in a similar fashion.

| Pin | JSON ID | Description                                 |
|-----|---------|---------------------------------------------|
| 0   | thfp    | Forward Port Thruster (front right)         |
| 1   | thfs    | Forward Starboard Thruster (front left)     |
| 2   | thap    | Aft Port Thruster (back left)               |
| 3   | thas    | Aft Starboard Thruster (back right)         |
| 4   | tvfp    | Top Forward Port Thruster (front right)     |
| 5   | tvfs    | Top Forward Starboard Thruster (front left) |
| 6   | tvap    | Top Aft Port Thruster (back left)           |
| 7   | tvas    | Top Aft Starboard Thruster (back right)     |
| 8   | mg      | Arm Gripper Motor                           |
| 9   | tm      | Micro ROV Thruster                          |
| 10  | mc      | Micro ROV return cord                       |


### Arduino I

This Arduino is for reading all the sensors on the ROV.

| JSON ID        | Description                            |
|----------------|----------------------------------------|
| simux          | X Orientation                          |
| simuy          | Y Orientation                          |
| simuz          | Z Orientation                          |
| simut          | Internal ROV temperature               |
| simuax         | Linear acceleration X                  |
| simuay         | Linear acceleration Y                  |
| simuaz         | Linear acceleration Z                  |
| sdepp          | External water pressure                |
| sdept          | External temperature                   |
| sdepd          | Depth                                  |
| sdepa          | Altitude                               |



### Ret Codes

| Return Code   | Description                                               |
|---------------|-----------------------------------------------------------|
|    0          | No Error                                                  |
|    1          | Outputs Halted.                                           |
|    2          | Left limit hit. Motor stopped                             |
|    3          | Right limit hit. Motor stopped                            |
|    4          | Arduino Booting                                           |
|   -1          | Incoming value out of range                               |
|   -2          | IMU BNO055 not found. Check wiring                        |
|   -3          | IMU BNO055 not initialised                                |
|   -4          | Depth Sensor not found. Check wiring                      |
|   -5          | Depth sensor not initialised                              |
|   -6          | getOutput method doesn't have an option for Arduino       |
|   -7          | getInput method doesn't have an option for Arduino        |
|   -8          | Output device ID is not valid                             |
|   -9          | Input device ID is not valid                              |
|   -10         | Can't call stopOutputs from a non-output Arduino          |
|   -11         | JSON parsing failed                                       |
|   -12         | Arduino ID not set up.                                    |
|   -13         | No message received in the last second. Outputs Halted.   |
|   -14         | RTD High Threshold                                        |
|   -15         | RTD Low Threshold                                         |
|   -16         | REFIN- > 0.85 x Bias                                      |
|   -17         | REFIN- < 0.85 x Bias - FORCE- open                        |
|   -18         | RTDIN- < 0.85 x Bias - FORCE- open                        |
|   -19         | Under/Over voltage                                        |
|   -20         | Sonar not initialised                                     |
|   -21         | Sonar could not update                                    |
|   -22         | Sonar not connected                                       |
|   -23         | Index not valid for setting sensor parameter              |

