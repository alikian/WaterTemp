#include <WiFiSave.h>

/********************************************************************/
// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>

#define DEBUG 1

// Libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
/********************************************************************/

WiFiSave wifiSave("WIFID34EB1","CI1GJ3Q7ZVZTIW96");

// DHT 11 sensor
#define DHTPIN 5
#define DHTTYPE DHT22

// WiFi parameters
#define WLAN_SSID       "WIFID34EB1"
#define WLAN_PASS       "CI1GJ3Q7ZVZTIW96"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
//#define AIO_SERVER      "52.5.238.97"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "alikian"
#define AIO_KEY         "ea6f80b7cb5b3ddd26f5dd29bde874e2b1beac0d"

// Functions
void connect();

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;


// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);/****************************** Feeds ***************************************/

// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish test = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test22");


void setup(void)
{
  // start serial port
  Serial.begin(115200);

  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

//unsigned long startWifi = millis();
//if(  WiFi.waitForConnectResult() != WL_CONNECTED) {
//  Serial.print(F("AutoConnect failed "));
// 
//  WiFi.begin(WLAN_SSID, WLAN_PASS);
//  WiFi.setAutoConnect(true);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(F("."));
//  }
//}
//unsigned long endWifi = millis();
//   Serial.print("duration to wifi: ");
//   Serial.println(endWifi-startWifi);
//  Serial.println();


  wifiSave.beginConnect();
  
  

  // connect to adafruit io
     unsigned long start = millis();
  connect();
     unsigned long duration = millis() - start;
   Serial.print("duration to mqtt: ");
   Serial.println(duration);





  if (! test.publish(123))
    Serial.println(F("Failed to publish battery"));
  else
    Serial.println(F("Battery temp published!"));

  client.flush();
  
  if (! mqtt.disconnect())
    Serial.println(F("Failed to disconnect"));
  else
    Serial.println(F("mqtt disconnected!"));
  Serial.println("DONE");


  ESP.deepSleep(20e6); // 20e6 is 20 microseconds
}
void loop(void)
{


}

// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}
