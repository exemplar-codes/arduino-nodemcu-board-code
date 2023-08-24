// connect to the Wifi
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const char *ssid = "Ahmar";
const char *password = "808@4443022";

void WIFI_setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {

    delay(1000);
    Serial.print("Attempting wifi connection...");

     Serial.print("Connected to WiFi!");
  }
}

// server discovery START
char serverUrl[100]; // address of working server

// forEach with each URL 'http://192.168.0.100:4000'
// runs a function for each
void cycleThroughURLs(void (*CallbackFunction)(char *), const char *baseURL, const char *port, int initialIP, int numVariations) {
    if (baseURL == NULL) baseURL = "http://192.168.0.";
    if (port == NULL) port = ":4000";
    if (initialIP == 0) initialIP = 100;
    if (numVariations == 0) numVariations = 11;

    for (int i = 0; i < numVariations; i++) {
        char generatedURL[100]; // Adjust the size as needed
        sprintf(generatedURL, "%s%d%s", baseURL, initialIP + i, port);

        if (CallbackFunction != NULL) {
            CallbackFunction(generatedURL);
        } else {
            Serial.println(generatedURL);
        }
    }
}

// checks if device at URL is active

  WiFiClient wifiClient;
  HTTPClient http; // Declare an object of class HTTPClient
int checkIfURLIsLive(char *url) {
    http.begin(wifiClient, url);
    int httpCode = http.GET();
    Serial.println("Pinged URL: ");
    Serial.println(url);
    Serial.println(", result: ");
    Serial.println(httpCode);

    return httpCode >= 200 && httpCode < 300;
}

// mutates global 'serverUrl' variable, if condition is met (device responds fine)
void setFoundServerURL(char *url) {
    int foundServer = checkIfURLIsLive(url); // http
    if(foundServer)
    {
        strcpy(serverUrl, url);
        Serial.println("Found server URL: ");
        Serial.println(url);
    }
}

// main server discovery function
void findServerInNetwork() {
    cycleThroughURLs(setFoundServerURL, NULL, NULL, 0, 0);
}

// serverUrl is discovered
// server discovery END

bool getValueFromWifi() // i.e. server
{
  bool serverSwitchState = false;

  if (WiFi.status() == WL_CONNECTED)
  { // Check WiFi connection status
    WiFiClient wifiClient;

    HTTPClient http; // Declare an object of class HTTPClient

    http.begin(wifiClient, serverUrl); // Specify request destination
    // http.begin(wifiClient, "http://jsonplaceholder.typicode.com/users/1"); // Specify request destination
    // http.begin(wifiClient, "https://api.github.com/users/sanjarcode"); // Specify request destination
    int httpCode = http.GET(); // Send the request

    Serial.println("Http code: "); // test
    Serial.println(httpCode); // test

    if (httpCode > 0)
    { // Check the returning code

      String payload = http.getString(); // Get the request response payload
      Serial.println(payload);           // Print the response payload

      if (payload == "true" || payload == "on")
        serverSwitchState = true;
    }
    else
    {
      Serial.println("Error. Http code: " + httpCode);
      serverSwitchState = false; // TURN OFF if communication is not possible
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
  findServerInNetwork();
  lastServerSwitchState = getValueFromWifi();
}

void loop()
{
  bool freshServerSwitchState = getValueFromWifi();
  if (freshServerSwitchState == lastServerSwitchState)
    return; // do nothing

  lastServerSwitchState = freshServerSwitchState; // update board value

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
