//
// Created by  changjin on 2021/02/25.
//

#ifndef OTA_ESP32_OTA_H
#define OTA_ESP32_OTA_H

#include <WiFi.h>
#include <Wire.h>
#include <ESPmDNS.h>
#include <TelnetStream.h>
#include "esp_wps.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define ESP_WPS_MODE      WPS_TYPE_PBC
#define ESP_MANUFACTURER  "ESPRESSIF"
#define ESP_MODEL_NUMBER  "ESP32"
#define ESP_MODEL_NAME    "low house IOT"
#define ESP_DEVICE_NAME   "low house Workshop Controller"

static esp_wps_config_t config;

// NTP
const long  gmtOffset_sec = 9 * 60 * 60;
const int daylightOffset_sec = 0;
const char* ntpServer = "pool.ntp.org";

void printLocalTime()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%Y %H:%M:%S");
}


//=========== begin oled ==============================

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin 4 # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D, 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//====== end of  oled ===================================
char g_nameprefix[100];
bool oledEnabled = false;
int numLines = 0;

//=====================================


void wpsInitConfig(){
    config.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
    config.wps_type = ESP_WPS_MODE;
    strcpy(config.factory_info.manufacturer, ESP_MANUFACTURER);
    strcpy(config.factory_info.model_number, ESP_MODEL_NUMBER);
    strcpy(config.factory_info.model_name, ESP_MODEL_NAME);
    strcpy(config.factory_info.device_name, ESP_DEVICE_NAME);
}

String wpspin2string(uint8_t a[]){
    char wps_pin[9];
    for(int i=0;i<8;i++){
        wps_pin[i] = a[i];
    }
    wps_pin[8] = '\0';
    return (String)wps_pin;
}

void ota_handle( void * parameter ) {
    for (;;) {
        ArduinoOTA.handle();
        delay(3500);
    }
}




void i2cscan(TwoWire *twi)
{
    byte error, address;
    int nDevices;
    TwoWire wire = *twi;

    Serial.println("Scanning...");

    nDevices = 0;
    for(address = 1; address < 127; address++ )
    {

        wire.beginTransmission(address);
        error = wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.print(address,HEX);
            Serial.println("  !");

            nDevices++;
        }
        else if (error==4)
        {
            Serial.print("Unknown error at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.println(address,HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}


void print(const String &s)
{
    if(oledEnabled) {

        if(numLines >= 8) {
            display.clearDisplay();
            display.setCursor(0,0);
            numLines = 0;
        }


        display.print(s);
        display.display();
    }else {
        Serial.print(s);
    }

    if(TelnetStream.available()) {
        TelnetStream.print(s);
    }


}

void print(const Printable& x)
{
    if(oledEnabled) {

        if(numLines >= 8) {
            display.clearDisplay();
            display.setCursor(0,0);
            numLines = 0;
        }

        display.print(x);
        display.display();
    }else {
        Serial.print(x);
    }

    if(TelnetStream.available()) {
        TelnetStream.print(x);
    }
}

void print(const char str[]) {
    if(oledEnabled) {
        if(numLines >= 8) {
            display.clearDisplay();
            display.setCursor(0,0);
            numLines = 0;
        }

        display.print(str);
        display.display();
    }else {
        Serial.print(str);
    }

    if(TelnetStream.available()) {
        TelnetStream.print(str);
    }
}

void print(int i) {
    if(oledEnabled) {
        if(numLines >= 8) {
            display.clearDisplay();
            display.setCursor(0,0);
            numLines = 0;
        }

        display.print(i);
        display.display();
    }else {
        Serial.print(i);
    }

    if(TelnetStream.available()) {
        TelnetStream.print(i);
    }
}


void print(unsigned long i) {
    if(oledEnabled) {
        if(numLines >= 8) {
            display.clearDisplay();
            display.setCursor(0,0);
            numLines = 0;
        }

        display.print(i);
        display.display();
    }else {
        Serial.print(i);
    }

    if(TelnetStream.available()) {
        TelnetStream.print(i);
    }
}

void print( float i) {
    if(oledEnabled) {
        if(numLines >= 8) {
            display.clearDisplay();
            display.setCursor(0,0);
            numLines = 0;
        }

        display.print(i);
        display.display();
    }else {
        Serial.print(i);
    }

    if(TelnetStream.available()) {
        TelnetStream.print(i);
    }
}


void println(void) {
    print("\n");
    if(TelnetStream.available()) {
        TelnetStream.print("\n");
    }
    numLines++;
}

void println(int i) {
    print(i);
    println();
}

void println(unsigned long i) {
    print(i);
    println();
}

void println(const Printable& x) {
    print(x);
    println();
}

void println(const char str[]) {
    print(str);
    println();
}

void println(const String &s) {
    print(s);
    println();
}


void setupOTA() {
    // Configure the hostname
    uint16_t maxlen = strlen(g_nameprefix) + 7;
    char *fullhostname = new char[maxlen];
    uint8_t mac[6];
    WiFi.macAddress(mac);
    snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", g_nameprefix, mac[3], mac[4], mac[5]);
    ArduinoOTA.setHostname(fullhostname);
    delete[] fullhostname;

    // Configure and start the WiFi station
//    WiFi.mode(WIFI_STA);
//    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // Port defaults to 3232
    // ArduinoOTA.setPort(3232); // Use 8266 port if you are working in Sloeber IDE, it is fixed there and not adjustable

    // No authentication by default
//     ArduinoOTA.setPassword("hayoung");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        //NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("\nAuth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("\nBegin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("\nConnect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("\nReceive Failed");
        else if (error == OTA_END_ERROR) Serial.println("\nEnd Failed");
    });

    ArduinoOTA.begin();

    println("OTA Initialized");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    xTaskCreate(
        ota_handle,          /* Task function. */
        "OTA_HANDLE",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL
    );            /* Task handle. */
}

void WiFiEvent(WiFiEvent_t event, system_event_info_t info){
    switch(event){
        case SYSTEM_EVENT_STA_START:
            Serial.println("Station Mode Started");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.println("Connected to :" + String(WiFi.SSID()));
            print("IP: ");
            println(WiFi.localIP());

            setupOTA();
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from station, attempting reconnection");
            WiFi.reconnect();

            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            println("WPS Successfull, stopping WPS and connecting to: " + String(WiFi.SSID()));
            esp_wifi_wps_disable();
            delay(10);
            WiFi.begin();
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            Serial.println("WPS Failed, retrying");
            esp_wifi_wps_disable();
            esp_wifi_wps_enable(&config);
            esp_wifi_wps_start(0);
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            println("WPS Timedout, retrying");
            delay(1000);
//            WiFi.disconnect(false, true);
            esp_wifi_wps_disable();
            esp_wifi_wps_enable(&config);
            esp_wifi_wps_start(0);

//            ESP.restart();
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            Serial.println("WPS_PIN = " + wpspin2string(info.sta_er_pin.pin_code));
            break;
        default:
            break;
    }
}

void setupWPS_OTA(const char* nameprefix) {
    size_t len  = strlen(nameprefix);
    for (int i = 0; i < len; ++i) {
        g_nameprefix[i] = nameprefix[i];
        g_nameprefix[i+1] = 0;
    }

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    // 임시로 사용하셈
//    i2cscan(&Wire);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
    }else {
        oledEnabled = true;
        display.display();
//        delay(1000);
        display.clearDisplay();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);
    }

    // 아래는 반드시 호출해야 함. 호출안하면 두번에 1번씩 연결이 실패함.
    WiFi.disconnect(false, false);

    WiFi.onEvent(WiFiEvent);
    // it reconnect with last success connection info.
    WiFi.begin();
    int retryCnt = 0;
    while (WiFi.status() != WL_CONNECTED) {
        if(retryCnt > 5) {
            println("not connected yet!");
            // 두번쨰 인자를 true 로 해 놓으면 공유기가 변경됐을때 이전 공유기롤 접속하려는걸 막을 수 있음.
            WiFi.disconnect(false, true);
            WiFi.mode(WIFI_MODE_STA);
            println("Starting WPS");

            wpsInitConfig();
            esp_wifi_wps_enable(&config);
            esp_wifi_wps_start(0);
            break;
        }
        delay(1000);
        Serial.println("Connecting to WiFi..");
        retryCnt++;
    }

    if(retryCnt <= 5) {
        TelnetStream.begin();

        println(" connected !");

        // ntp 연결
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            println("Failed to obtain time");
            return;
        }

        print(timeinfo.tm_hour);
        print(":");
        print(timeinfo.tm_min);
        print(":");
        print(timeinfo.tm_sec);
        println("");


    }

}


#endif //OTA_ESP32_OTA_H
