#ifndef VARIABLES_H
#define VARIABLES_H
#include <Arduino.h>

/*
These two params are import for calibration.
Due to sensetivety of vibrationsensor and build quality of the pad these params possibly need to change to make the program work without vibration interference.
Params can also be connected to analog inputs (e.g. Potentiometers) to control the values. To do that uncomment the following lines: 
main.cpp: 10,11,36,37

Volume can also be changed by analog input. Uncomment the following lines:
main.cpp: 26,36
*/

//PARAMS FOR CALIBRATION
uint8_t VBTreshold = 30;
uint8_t DelayTime = 100;

//OTHER PARAMS
bool reset = true;
int oldVbsValue;
int volume = 127;

unsigned long currentTime;
unsigned long prevTime = 0;

#endif //VARIABLES_H