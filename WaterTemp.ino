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
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "alikian"
#define AIO_KEY         "ea6f80b7cb5b3ddd26f5dd29bde874e2b1beac0d"

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Functions
void connect();

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
//const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;

// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);/****************************** Feeds ***************************************/

// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");

Adafruit_MQTT_Publish waterTemp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/waterTemp");

Adafruit_MQTT_Publish battery = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/battery");

void setup(void)
{
  // start serial port
  Serial.begin(115200);
  sensors.begin();

  // Init sensor
  dht.begin();

  Serial.println(F("Adafruit IO Example"));



  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  /********************************************************************/
  Serial.print(" Requesting temperatures...");

  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.println("DONE");
  /********************************************************************/
  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?


  float batteryLevel = analogRead(A0) * 117. / 1024.;

  // Grab the current state of the sensor
  int humidity_data = (int)dht.readHumidity();
  int temperature_data = (int)dht.readTemperature();

  Serial.println("");
  Serial.print("Battery: ");
  Serial.println(batteryLevel);
  Serial.println("DHT22");
  Serial.print("humidity_data: ");
  Serial.println(humidity_data);
  Serial.print("temperature_data: ");
  Serial.println(temperature_data);

  // connectWiFi();
  wifiSave.beginConnect();
  
  // connect to adafruit io
  connectMqtt();

  if (! humidity.publish(123))
    Serial.println(F("Failed to publish humidity"));
  else
    Serial.println(F("Humidity published!"));

  // Publish data
//  if (! humidity.publish(humidity_data))
//    Serial.println(F("Failed to publish humidity"));
//  else
//    Serial.println(F("Humidity published!"));
//
//  if (! waterTemp.publish(sensors.getTempCByIndex(0)))
//    Serial.println(F("Failed to publish water temp"));
//  else
//    Serial.println(F("Water temp published!"));
//
//  if (! battery.publish(batteryLevel))
//    Serial.println(F("Failed to publish battery"));
//  else
//    Serial.println(F("Battery temp published!"));
//
//  if (! temperature.publish(temperature_data))
//    Serial.println(F("Failed to publish temperature"));
//  else
//    Serial.println(F("Temperature published!"));

  client.flush();

  if (! mqtt.disconnect())
    Serial.println(F("Failed to disconnect"));
  else
    Serial.println(F("mqtt disconnected!"));

  ESP.deepSleep(60e6); // 20e6 is 20 microseconds
}
void loop(void)
{


}

void connectWiFi() {
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

// connect to adafruit io via MQTT
void connectMqtt() {

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
