#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

char* ssid     = "WIFID34EB1";
char* password = "CI1GJ3Q7ZVZTIW96";

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "alikian"
#define AIO_KEY         "ea6f80b7cb5b3ddd26f5dd29bde874e2b1beac0d"


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;


// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);/****************************** Feeds ***************************************/

// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish test2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test2");


void setup() {
  // start serial port
  Serial.begin(115200);
  delay(10);
  Serial.println(F("Adafruit IO Example"));

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  unsigned long StartTime = millis();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  unsigned long EndTime = millis();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                 " Seconds");

  if (! test2.publish(121))
    Serial.println(F("Failed to publish battery"));
  else
    Serial.println(F("Battery temp published!"));

  client.flush();

  if (! mqtt.disconnect())
    Serial.println(F("Failed to disconnect"));
  else
    Serial.println(F("mqtt disconnected!"));
  Serial.println("DONE");

  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
  Serial.println("This will never be printed");

}

void loop() {
  // put your main code here, to run repeatedly:

}
