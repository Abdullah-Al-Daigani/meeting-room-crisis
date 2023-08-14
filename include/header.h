// libraries:

#include <Arduino.h> // include the arduino platform
#include <string>    // include the String library
#include "qrcode.h"  // include the QR code generation library

// include necessary libraries for the epaper display to work
#define LILYGO_T5_V213                 //
#include <boards.h>                    //
#include <GxEPD.h>                     //
#include <GxDEPG0213BN/GxDEPG0213BN.h> // include necessary libraries for the E-paper display to function
#include <GxIO/GxIO_SPI/GxIO_SPI.h>    //
#include <GxIO/GxIO.h>                 //

#include <ArduinoJson.h>   // used to conveniently make and modify JSON files
#include <ArduinoJson.hpp> //

#include <WiFi.h>         // the library used to turn on and use WiFi
#include <PubSubClient.h> // the library managing the MQTT protocol to communicate with the Host over WiFi

const char *content = "no messages yet"; // declares and initializes a pointer to store the latest message recieved from Telegram
String message = "No messages yet";      // a variable used to store the incoming telegram message
unsigned long lastMsg = millis();        // a variable used to set the frequency of the data reports

#define BracketCount 4

int BracketRange[BracketCount];
bool ReservationStatus[BracketCount];
String CurrentReserveName = "SHTTLE";
String CurrentReservePeriod = "08:00 - 09:00";

const char *pCurrentReserveName = "SHTTLE";
const char *pCurrentReservePeriod = "08:00 - 09:00";

void reconnect();                  // declaration of the function used to reconnect to the MQTT broker
void displayLoop(float potin_per); // declaration of the display loop
void displayName(void);            //
void displayQRcode(void);          // declare functions to use later
void displaySC1(String name, String period);

// other wifi cred
const char *ssid = "W";            // WiFi ssid
const char *password = "12121212"; // WiFi password
// const char *mqtt_server = "192.168.8.179"; // MQTT broker ip address

// const char *ssid = "Nutmeg-2.4GHz";       // WiFi ssid
// const char *password = "Qw121212";        // WiFi password
// const char *mqtt_server = "192.168.1.12"; // MQTT broker ip address

// objects generation:                                                // the object used to communicate with the power monitor module
WiFiClient espClient;                         // initialization of the onboard WiFi client
PubSubClient client(espClient);               // the initialization of the PubSubClient used for the MQTT protocol
GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET); //
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);  // construct the display object to control the E-paper display
