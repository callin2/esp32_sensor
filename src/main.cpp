#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>
#include "ESP32_OTA.h"
#include "luainc.h"

#include <CheapStepper.h>


static lua_State *L = luaL_newstate();
const char* errmsg = NULL;


//=======

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

unsigned long delayTime;


CheapStepper stepper (12,14,27,26);

boolean moveClockwise = true;


void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Booting");
    setupWPS_OTA("LRH_Workshop");


    if (! bme.begin(0x76, &Wire)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }


    Serial.println("-- Weather Station Scenario --");
    Serial.println("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling,");
    Serial.println("filter off");
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF   );

    // suggested rate is 1/60Hz (1m)
    delayTime = 60000; // in milliseconds


    stepper.setRpm(10);

    Serial.println();
    Serial.print(stepper.getRpm()); // get the RPM of the stepper
    Serial.print(" rpm = delay of ");
    Serial.print(stepper.getDelay()); // get delay between steps for set RPM
    Serial.print(" microseconds between steps");
    Serial.println();

    delay(1000);

    moveClockwise = false;

    // and move back to the start position (0 degree)

    stepper.moveToDegree (moveClockwise, 0);
    delay(1000);



}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}


void loop() {
  // put your main code here, to run repeatedly:

//    bme.takeForcedMeasurement(); // has no effect in normal mode
//
//    printValues();
//    delay(delayTime);

    //====================
    moveClockwise = true;

    // let's move the stepper clockwise to position 2048
    // which is 180 degrees, a half-turn (if using default of 4096 total steps)

    stepper.moveTo (moveClockwise, 2048);

    delay(1000); // wait a sec

    // and now let's move another 90 degrees (a quarter-turn) clockwise

    stepper.moveDegrees (moveClockwise, 90);

    delay(1000);

    moveClockwise = false;

    stepper.moveToDegree(moveClockwise, 0);

}