#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>
#include "ESP32_OTA.h"
#include "luainc.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>


String token = "1019497337:AAFuG0Ju8ejug8mlyBp0KbvLh5V4E-cJQSw";
String CHAT_ID = "703642793";


static lua_State *L = luaL_newstate();
const char* errmsg = NULL;

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


//=======

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

unsigned long delayTime;

const char* test_root_ca = \
                           "-----BEGIN CERTIFICATE-----\n"
                           "MIIEADCCAuigAwIBAgIBADANBgkqhkiG9w0BAQUFADBjMQswCQYDVQQGEwJVUzEh\n"
                           "MB8GA1UEChMYVGhlIEdvIERhZGR5IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBE\n"
                           "YWRkeSBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA0MDYyOTE3\n"
                           "MDYyMFoXDTM0MDYyOTE3MDYyMFowYzELMAkGA1UEBhMCVVMxITAfBgNVBAoTGFRo\n"
                           "ZSBHbyBEYWRkeSBHcm91cCwgSW5jLjExMC8GA1UECxMoR28gRGFkZHkgQ2xhc3Mg\n"
                           "MiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCCASAwDQYJKoZIhvcNAQEBBQADggEN\n"
                           "ADCCAQgCggEBAN6d1+pXGEmhW+vXX0iG6r7d/+TvZxz0ZWizV3GgXne77ZtJ6XCA\n"
                           "PVYYYwhv2vLM0D9/AlQiVBDYsoHUwHU9S3/Hd8M+eKsaA7Ugay9qK7HFiH7Eux6w\n"
                           "wdhFJ2+qN1j3hybX2C32qRe3H3I2TqYXP2WYktsqbl2i/ojgC95/5Y0V4evLOtXi\n"
                           "EqITLdiOr18SPaAIBQi2XKVlOARFmR6jYGB0xUGlcmIbYsUfb18aQr4CUWWoriMY\n"
                           "avx4A6lNf4DD+qta/KFApMoZFv6yyO9ecw3ud72a9nmYvLEHZ6IVDd2gWMZEewo+\n"
                           "YihfukEHU1jPEX44dMX4/7VpkI+EdOqXG68CAQOjgcAwgb0wHQYDVR0OBBYEFNLE\n"
                           "sNKR1EwRcbNhyz2h/t2oatTjMIGNBgNVHSMEgYUwgYKAFNLEsNKR1EwRcbNhyz2h\n"
                           "/t2oatTjoWekZTBjMQswCQYDVQQGEwJVUzEhMB8GA1UEChMYVGhlIEdvIERhZGR5\n"
                           "IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBEYWRkeSBDbGFzcyAyIENlcnRpZmlj\n"
                           "YXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQAD\n"
                           "ggEBADJL87LKPpH8EsahB4yOd6AzBhRckB4Y9wimPQoZ+YeAEW5p5JYXMP80kWNy\n"
                           "OO7MHAGjHZQopDH2esRU1/blMVgDoszOYtuURXO1v0XJJLXVggKtI3lpjbi2Tc7P\n"
                           "TMozI+gciKqdi0FuFskg5YmezTvacPd+mSYgFFQlq25zheabIZ0KbIIOqPjCDPoQ\n"
                           "HmyW74cNxA9hi63ugyuV+I6ShHI56yDqg+2DzZduCLzrTia2cyvk0/ZM/iZx4mER\n"
                           "dEr/VxqHD3VILs9RaRegAhJhldXRQLIQTO7ErBBDpqWeCtWVYpoNz4iCxTIM5Cuf\n"
                           "ReYNnyicsbkqWletNw+vHX/bvZ8=\n"
                           "-----END CERTIFICATE-----\n";

WiFiClientSecure client;

UniversalTelegramBot bot(token, client);


// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
    println("handleNewMessages");
    println(String(numNewMessages));

    bme.takeForcedMeasurement();

    for (int i=0; i<numNewMessages; i++) {
        // Chat id of the requester
        String chat_id = String(bot.messages[i].chat_id);
        if (chat_id != CHAT_ID){
            bot.sendMessage(chat_id, "Unauthorized user", "");
            continue;
        }

        // Print the received message
        String text = bot.messages[i].text;
        Serial.println(text);

        String from_name = bot.messages[i].from_name;

        if (text == "/start") {
            String welcome = "Welcome, " + from_name + ".\n";
            welcome += "Use the following commands to control your outputs.\n\n";
            welcome += "/led_on to turn GPIO ON \n";
            welcome += "/led_off to turn GPIO OFF \n";
            welcome += "/state to request current GPIO state \n";
            bot.sendMessage(chat_id, welcome, "");
        }

        if (text == "/temp") {
            print("temp");

            String x = String("Temperature = ");
            String y = String(bme.readTemperature());
            String z = String(" °C");

            bot.sendMessage(chat_id, (x+y+z).c_str(), "");

        }

        if (text == "/humid") {
            print("humid");
            String x = String("Humidity = ");
            String y = String(bme.readHumidity());
            String z = String(" %");

            bot.sendMessage(chat_id, (x+y+z).c_str(), "");
        }

        if (text == "/press") {
            String x = String("Pressure = ");
            String y = String(bme.readPressure() / 100.0F);
            String z = String(" hPa");

            bot.sendMessage(chat_id, (x+y+z).c_str(), "");
        }


    }
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Booting");
    setupWPS_OTA("LRH_Workshop");

    client.setCACert( test_root_ca );


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

}




void printValues() {
    print("Temperature = ");
    print(bme.readTemperature());
    println(" *C");

    print("Pressure = ");

    print(bme.readPressure() / 100.0F);
    println(" hPa");

    print("Approx. Altitude = ");
    print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    println(" m");

    print("Humidity = ");
    print(bme.readHumidity());
    println(" %");

    println();
}


void loop() {
  // put your main code here, to run repeatedly:

     // has no effect in normal mode

//    printValues();
//    delay(delayTime);

    if (millis() > lastTimeBotRan + botRequestDelay)  {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while(numNewMessages) {
            println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }

}