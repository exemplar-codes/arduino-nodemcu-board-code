// connect to the Wifi
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char *ssid = "yourSSID";
const char *password = "yourPassword";

void WIFI_setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {

    delay(1000);
    Serial.print("Connecting..");
  }
}

bool getValueFromWifi() // i.e. server
{
  bool serverSwitchState = false;

  if (WiFi.status() == WL_CONNECTED)
  { // Check WiFi connection status
    WiFiClient wifiClient;

    HTTPClient http; // Declare an object of class HTTPClient

    http.begin(wifiClient, "http://192.168.0.102:3000"); // Specify request destination
    // http.begin(wifiClient, "http://jsonplaceholder.typicode.com/users/1"); // Specify request destination
    // http.begin(wifiClient, "https://api.github.com/users/sanjarcode"); // Specify request destination
    int httpCode = http.GET(); // Send the request

    if (httpCode > 0)
    { // Check the returning code

      String payload = http.getString(); // Get the request response payload
      Serial.println(payload);           // Print the response payload

      if (payload == "true")
        serverSwitchState = true;
    }
    else
    {
      Serial.println("Error. Http code: " + httpCode);
    }

    http.end(); // Close connection
  }

  return serverSwitchState;
}

// poll the server and switch relay as needed
// update the external PIN to the relay's value (LED should be ON if relay is ON)

#define RELAY_PIN 5 // D1 pin on board

bool lastServerSwitchState = false;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  WIFI_setup();
  lastServerSwitchState = getValueFromWifi();
}

void loop()
{
  bool freshServerSwitchState = getValueFromWifi();
  if (freshServerSwitchState == lastServerSwitchState)
    return; // do nothing

  // // turn OFF the relay
  // // and wait
  // digitalWrite(RELAY_PIN, LOW);
  // digitalWrite(LED_BUILTIN, !digitalRead(RELAY_PIN));
  // delay(1000);

  // // turn ON the relay
  // // and wait
  // digitalWrite(RELAY_PIN, HIGH);
  // digitalWrite(LED_BUILTIN, !digitalRead(RELAY_PIN));
  // delay(1000);

  bool relayValue_sane = !freshServerSwitchState;
  // since relay is active LOW, but I find active HIGH to be more comfortable

  digitalWrite(RELAY_PIN, relayValue_sane);           // true means ON, false means OFF (sane)
  digitalWrite(LED_BUILTIN, !digitalRead(RELAY_PIN)); // update as per relay value
  delay(1000);                                        // poll every 5 second
}
