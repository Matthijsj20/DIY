/*
This sketch can be used as an MIDI drumpad. De sketch uses an analog input signal to read if the drumpad is triggerd.
All pins can be changed in pin.h. 
For calibration the variables in variables.h can be changed. 
When the sensor exceeds the treshold the sketch is sending a midi signal via the native USB port of the micro controller. Currently the sketches is sending C3 as note.
Obviously this can be changed. Every note is defined in de MidiCommands.h header. The note can be changed by changing the second variable of the midiCommand() function 
on rule 54 of main.cpp. 
When using the drumpad computer software is also needed for reading the MIDI signal. Almost every DAW can read MIDI signals these days. When the USB is plugged in the 
drumpad wil be visable as an MIDI USB device (Device probably has the same name as the microcontroller). \
Select this device in the MIDI software and attach the desirend sound to the note wich is beeing sended on rule 54 of main.cpp
When using windows: Most of the time standard windows audio drivers are having really high latency when using midi signals in a DAW. If this is the case install ASIO driver. 
Using ASIO should fix the problem.
*/

#include "MidiCommands.h"
#include "pin.h"
#include "PotMeter.h"
#include "vibrationSensor.h"
#include "variables.h"

#include <MIDIUSB.h>

vibrationSensor vbs(VibSenPin);
//PotMeter potTreshold(PotTresholdPin);
//PotMeter potDelay(PotDelayPin);
//PotMeter potVolume(PotVolumePin);

void setup()
{
}

void midiCommand(byte cmd, byte data1, byte data2)
{
    // First parameter is the event type (top 4 bits of the command byte).
    // Second parameter is command byte combined with the channel.
    // Third parameter is the first data byte
    // Fourth parameter second data byte

    midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
    MidiUSB.sendMIDI(midiMsg);
}
void loop()
{
    //VBTreshold = potTreshold.readPotMeter();
    //DelayTime = map(potDelay.readPotMeter(), 0, 1023, 0, 1000); //Min 0, max 1000 milliseconds
    //volume = map(potVolume.readPotMeter(), 0, 1023, 0, 127); //Min 0, Max 127. 127 is highest volume level in MIDI.

    currentTime = millis();

    int vbsValue = vbs.readVibrationSensor();
    if (vbsValue > VBTreshold)
    {
        if (currentTime - prevTime > DelayTime)
        {
            midiCommand(0x90, NoteC3, volume);
            prevTime = currentTime;
            MidiUSB.flush();
        }
    }
}
