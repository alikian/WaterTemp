#include "AdafruitIO_WiFi.h"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "alikian"
#define AIO_KEY         "ea6f80b7cb5b3ddd26f5dd29bde874e2b1beac0d"
// WiFi parameters
#define WLAN_SSID       "WIFID34EB1"
#define WLAN_PASS       "CI1GJ3Q7ZVZTIW96"



AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WLAN_SSID, WLAN_PASS);


// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "alikian"
#define AIO_KEY         "ea6f80b7cb5b3ddd26f5dd29bde874e2b1beac0d"

// this int will hold the current count for our sketch
int count = 0;

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("counter");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  Serial.print("sending -> ");
  Serial.println(count);
  
  unsigned long start = millis();
  counter->save(count);
  unsigned long end = millis();

  Serial.print("duration -> ");
  Serial.println(end-start);

  delay(1000);

  ESP.deepSleep(10e6); // 20e6 is 20 microseconds
}
void loop(void)
{


}

