// connect to the Wifi
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <stdio.h>
#include <string.h>

const char *ssid = "Ahmar";
const char *password = "808@4443022";

void WIFI_setup() // good
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Attempting wifi connection...");
  }

  Serial.println("Connected to WiFi!");
}

// server discovery START
char serverUrl[100]; // address of working server

// forEach with each URL 'http://192.168.0.100:4000'
// runs a function for each
void cycleThroughURLs(bool (*CallbackFunction)(char *), const char *baseURL, const char *port, int initialIP, int numVariations)
{
  if (baseURL == NULL)
    baseURL = "http://192.168.0.";
  if (port == NULL)
    port = ":4000";
  if (initialIP == 0)
    initialIP = 100;
  if (numVariations == 0)
    numVariations = 11;

  for (int i = 0; i < numVariations; i++)
  {
    char generatedURL[100]; // Adjust the size as needed
    sprintf(generatedURL, "%s%d%s", baseURL, initialIP + i, port);

    if (CallbackFunction != NULL)
    {
      bool success = CallbackFunction(generatedURL);
      if (success)
      {
        Serial.println("Server found, ending discovery.");
        return;
      } // return if server found
    }
    else
    {
      Serial.println(generatedURL);
    }
  }
}

// response parsing util
#include <iostream>
#include <string>
#include <vector>

std::vector<bool> parseBooleanArray(const std::string &json_string)
{
  std::vector<bool> bool_array;

  // Ensure the JSON array starts with '[' and ends with ']'
  if (json_string.empty() || json_string[0] != '[' || json_string.back() != ']')
  {
    std::cout << "Invalid JSON array format." << std::endl;
    return bool_array;
  }

  size_t current = 1; // Skip '['
  while (current < json_string.size() - 1)
  {
    if (json_string.compare(current, 4, "true") == 0)
    {
      bool_array.push_back(true);
      current += 4;
    }
    else if (json_string.compare(current, 5, "false") == 0)
    {
      bool_array.push_back(false);
      current += 5;
    }
    else
    {
      std::cout << "Invalid boolean value." << std::endl;
      return bool_array;
    }

    // Skip comma or whitespace
    while (current < json_string.size() - 1 &&
           (json_string[current] == ',' ||
            json_string[current] == ' ' ||
            json_string[current] == '\t' ||
            json_string[current] == '\n'))
    {
      ++current;
    }
  }

  return bool_array;
}

// checks if device at URL is active

WiFiClient wifiClient;
HTTPClient http; // Declare an object of class HTTPClient
int checkIfURLIsLive(char *url)
{

  http.begin(wifiClient, url);
  int httpCode = http.GET();
  Serial.print("Ping potential server at: ");
  Serial.print(url);
  Serial.print(", result (httpCode): ");
  Serial.println(httpCode);

  return httpCode >= 200 && httpCode < 300;
}

// mutates global 'serverUrl' variable, if condition is met (device responds fine)
bool setFoundServerURL(char *url)
{
  int foundServer = checkIfURLIsLive(url); // http
  if (foundServer)
  {
    strcpy(serverUrl, url);
    Serial.println("Found server URL: ");
    Serial.println(url);
  }

  return foundServer; // to exit rediscovery remaining process if found
}

// main server discovery function
void findServerInNetwork()
{
  Serial.println("Starting server discovery...");
  cycleThroughURLs(setFoundServerURL, NULL, NULL, 0, 0);
}

// serverUrl is discovered
// server discovery END

int TRIES_BEFORE_REDISCOVERY = 20; // 10 seconds
int tries_left = TRIES_BEFORE_REDISCOVERY;
std::vector<bool> getValuesFromWifi() // i.e. server
{
  // bool serverSwitchState = false;
  std::vector<bool> serverSwitchStates(4, false);

  if (WiFi.status() == WL_CONNECTED)
  { // Check WiFi connection status
    WiFiClient wifiClient;

    HTTPClient http; // Declare an object of class HTTPClient

    http.begin(wifiClient, serverUrl); // Specify request destination
    // http.begin(wifiClient, "http://jsonplaceholder.typicode.com/users/1"); // Specify request destination
    // http.begin(wifiClient, "https://api.github.com/users/sanjarcode"); // Specify request destination
    int httpCode = http.GET(); // Send the request

    Serial.print("getValueFromWifi, Http code: "); // test
    Serial.println(httpCode);                      // test

    if (httpCode > 0)
    { // Check the returning code

      String payload = http.getString();  // Get the request response payload
      Serial.print("received payload: "); // Print the response payload
      Serial.println(payload);

      int array_length = 4;
      std::vector<bool> payloads = parseBooleanArray(std::string(payload.c_str()));

      // if (payload == "true" || payload == "on")
      //   serverSwitchState = true;
      for (int i = 0; i < array_length && i < 4; i++)
      {
        if (payloads.size() < array_length)
        { // not an array or wrong format, consider it an error
          Serial.println("Issue with parsing payload");
          serverSwitchStates[i] = false;
          continue;
        }

        serverSwitchStates[i] = payloads[i]; // update value (happy path)
      }

      if (tries_left <= 0)
      {
        tries_left = TRIES_BEFORE_REDISCOVERY; // success, scope of discovery is less.
        Serial.println("Rediscovery tries reset to max value.");
      }
    }
    else
    {
      Serial.println("getValueFromWifi error. Http code: " + httpCode);

      // serverSwitchState = false; // TURN OFF if communication is not possible
      for (int i = 0; i < 4; i++)
        serverSwitchStates[i] = false;

      tries_left--; // fail once

      Serial.println("Tries left until rediscovery: " + tries_left);

      if (tries_left <= 0)
      {
        findServerInNetwork();
      }
    }

    http.end(); // Close connection
  }

  return serverSwitchStates;
}

// poll the server and switch relay as needed
// update the external PIN to the relay's value (LED should be ON if relay is ON)

#define RELAY_PIN 5   // D1 pin on board
#define RELAY_PIN_1 5 // same as RELAY_PIN
#define RELAY_PIN_2 4 // D2 pin on board
#define RELAY_PIN_3 3 // D3 pin on board
#define RELAY_PIN_4 2 // D4 pin on board

std::vector<int> RELAY_PINS{RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4};

// bool lastServerSwitchState = false;
std::vector<bool> lastServerSwitchStates(4, false);
void setup()
{
  // Setup
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 4; ++i)
  {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], !false); // OFF (default)
  }
  WIFI_setup();

  // find server
  findServerInNetwork();
}

void loop()
{
  // Get values from WiFi
  // bool freshServerSwitchState = getValueFromWifi();
  std::vector<bool> freshServerSwitchStates = getValuesFromWifi();

  // bool stateNotChanged = freshServerSwitchState == lastServerSwitchState;
  bool stateNotChanged = (freshServerSwitchStates == lastServerSwitchStates);

  if (stateNotChanged)
  {
    Serial.println("State comparison: no change");
    delay(500); // prevent too frequent polling when can't find server
    return;     // do nothing
  }

  Serial.println("State comparison: changed");

  // lastServerSwitchState = freshServerSwitchState; // update board value
  lastServerSwitchStates = freshServerSwitchStates; // update board value

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

  // bool relayValue_sane = !freshServerSwitchState;
  std::vector<bool> relayValues_sane(freshServerSwitchStates);
  for (int i = 0; i < 4; i++)
    relayValues_sane[i] = !relayValues_sane[i];
  // since relay is active LOW, but I find active HIGH to be more comfortable

  // digitalWrite(RELAY_PIN, relayValue_sane);           // true means ON, false means OFF (sane)
  for (int i = 0; i < 4; i++)
  {
    bool value = relayValues_sane[i];
    digitalWrite(RELAY_PINS[i], value);
    std::cout << "value " << i << " " << value << std::endl;
  }

  // digitalWrite(LED_BUILTIN, !digitalRead(RELAY_PIN)); // update as per relay value
  // has only one LED, ignore

  delay(1000); // poll every 5 second
}
