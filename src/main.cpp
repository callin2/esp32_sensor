/*
 * Simple demo, should work with any driver board
 *
 * Connect STEP, DIR as indicated
 *
 * Copyright (C)2015-2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include <analogWrite.h>


// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

// All the wires needed for full functionality
#define DIR 19
#define STEP 18
//#define STEP 2
//Uncomment line to use enable/disable functionality
//#define SLEEP 13

// 2-wire basic config, microstepping is hardwired on the driver

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

void setup() {
//    stepper.begin(RPM, MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    Serial.begin(115200);

    pinMode(DIR, OUTPUT);
    pinMode(STEP, OUTPUT);

    analogWriteResolution(STEP, 8);

    digitalWrite(DIR, 1);
}

int brightStep = 1;
int brightness = 0;


// the loop routine runs over and over again forever:
void loop() {
    brightness += brightStep;
    if ( brightness == 0 || brightness == 511 ) {
        brightStep = -brightStep;
    }

    analogWrite(STEP, brightness);
    // wait for 30 milliseconds to see the dimming effect
    delay(100);
    Serial.println(brightness);
}

//void loop() {
//
//    analog(STEP, 511.0);
//    delay(100);
//
//}
