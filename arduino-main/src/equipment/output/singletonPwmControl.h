// #ifndef SINGLETON_PWM
// #define SINGLETON_PWM

// #include <Arduino.h>

// #include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>

// /*
//   The singleton pwm control class gives you access to a single object from which you can control anything connected to the servo control board
// */
// class SingletonPwm {

//   protected:
//     // Represents a motor controlled by an ESC
//     static Adafruit_PWMServoDriver motor;

//     SingletonPwm ();

//   public:

//     static SingletonPwm getInstance();

//     int setValue(int pin, int inputValue);

// };

// #endif