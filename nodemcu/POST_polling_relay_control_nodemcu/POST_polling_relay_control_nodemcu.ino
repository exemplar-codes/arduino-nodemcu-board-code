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
    Serial.println("Attempting wifi connection...");
  }

  Serial.println("Connected to WiFi!");
}

// server discovery START
char serverUrl[100]; // address of working server

// forEach with each URL 'http://192.168.0.100:4000'
// runs a function for each
void cycleThroughURLs(bool (*CallbackFunction)(char *), const char *baseURL, const char *port, int initialIP, int numVariations) {
    if (baseURL == NULL) baseURL = "http://192.168.0.";
    if (port == NULL) port = ":4000";
    if (initialIP == 0) initialIP = 100;
    if (numVariations == 0) numVariations = 11;

    for (int i = 0; i < numVariations; i++) {
        char generatedURL[100]; // Adjust the size as needed
        sprintf(generatedURL, "%s%d%s", baseURL, initialIP + i, port);

        if (CallbackFunction != NULL) {
            bool success = CallbackFunction(generatedURL);
            if(success) {
              Serial.println("Server found, ending discovery.");
              return;
            } // return if server found
        } else {
            Serial.println(generatedURL);
        }
    }
}

// response parsing util
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool* parseBooleanArray(const char *json_string, int *array_length) {
    // Ensure the JSON array starts with '[' and ends with ']'
    if (json_string[0] != '[' || json_string[strlen(json_string) - 1] != ']') {
        printf("Invalid JSON array format.\n");
        return NULL;
    }

    // Count the number of commas to determine the array length
    *array_length = 1; // At least one element
    for (const char *c = json_string; *c; ++c) {
        if (*c == ',') {
            ++(*array_length);
        }
    }

    bool *bool_array = (bool*) malloc(*array_length * sizeof(bool));
    if (bool_array == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    const char *current = json_string + 1; // Skip '['
    for (int i = 0; i < *array_length; ++i) {
        if (strncmp(current, "true", 4) == 0) {
            bool_array[i] = true;
            current += 4;
        } else if (strncmp(current, "false", 4) == 0) {
            bool_array[i] = false;
            current += 5;
        } else {
            printf("Invalid boolean value.\n");
            free(bool_array);
            return NULL;
        }

        // Skip comma or whitespace
        while (*current && (*current == ',' || *current == ' ' || *current == '\t' || *current == '\n')) {
            ++current;
        }
    }

    return bool_array;
}


// checks if device at URL is active

WiFiClient wifiClient;
HTTPClient http; // Declare an object of class HTTPClient
int checkIfURLIsLive(char *url) {

  http.begin(wifiClient, url);
  int httpCode = http.GET();
  Serial.print("Ping potential server at: ");
  Serial.print(url);
  Serial.print(", result (httpCode): ");
  Serial.println(httpCode);

  return httpCode >= 200 && httpCode < 300;
}

// mutates global 'serverUrl' variable, if condition is met (device responds fine)
bool setFoundServerURL(char *url) {
    int foundServer = checkIfURLIsLive(url); // http
    if(foundServer)
    {
        strcpy(serverUrl, url);
        Serial.println("Found server URL: ");
        Serial.println(url);
    }

    return foundServer; // to exit rediscovery remaining process if found
}

// main server discovery function
void findServerInNetwork() {
    Serial.println("Starting server discovery...");
    cycleThroughURLs(setFoundServerURL, NULL, NULL, 0, 0);
}

// serverUrl is discovered
// server discovery END

int TRIES_BEFORE_REDISCOVERY = 20; // 10 seconds
int tries_left = TRIES_BEFORE_REDISCOVERY;
bool* getValuesFromWifi() // i.e. server
{
  // bool serverSwitchState = false;
  bool serverSwitchStates[4] = {false, false, false, false};

  if (WiFi.status() == WL_CONNECTED)
  { // Check WiFi connection status
    WiFiClient wifiClient;

    HTTPClient http; // Declare an object of class HTTPClient

    http.begin(wifiClient, serverUrl); // Specify request destination
    // http.begin(wifiClient, "http://jsonplaceholder.typicode.com/users/1"); // Specify request destination
    // http.begin(wifiClient, "https://api.github.com/users/sanjarcode"); // Specify request destination
    int httpCode = http.GET(); // Send the request

    Serial.print("getValueFromWifi, Http code: "); // test
    Serial.println(httpCode); // test

    if (httpCode > 0)
    { // Check the returning code

      // String payload = http.getString(); // Get the request response payload
      // Serial.print("received payload: "); // Print the response payload
      // Serial.println(payload);

      int array_length = 4;
      bool *payloads = NULL; //parseBooleanArray(payload.c_str(), &array_length);

      // if (payload == "true" || payload == "on")
      //   serverSwitchState = true;
      for(int i = 0; i < array_length && i < 4; i++)
      {
        if(payloads == NULL) { // not an array or wrong format, consider response string
          // update all with the same value
          serverSwitchStates[i] = true;// payload == "true" || payload == "on";
          continue;
        }

        serverSwitchStates[i] = payloads[i];
      }

      if(tries_left <= 0)
      {
        tries_left = TRIES_BEFORE_REDISCOVERY; // success, scope of discovery is less.
        Serial.println("Rediscovery tries reset to max value.");
      }

      free(payloads);
    }
    else
    {
      Serial.println("getValueFromWifi error. Http code: " + httpCode);

      // serverSwitchState = false; // TURN OFF if communication is not possible
      for(int i = 0; i < 4; i++) serverSwitchStates[i] = false;

      tries_left--; // fail once

      Serial.println("Tries left until rediscovery: " + tries_left);

      if(tries_left <= 0)
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

int RELAY_PINS[4] = {RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4};

// bool lastServerSwitchState = false;
bool lastServerSwitchStates[4] = {false, false, false, false};
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  // pinMode(RELAY_PIN, OUTPUT); // doesn't make sense

  WIFI_setup();

  // lastServerSwitchState = getValueFromWifi();
  {
    bool* _ = getValuesFromWifi();
    for (int i = 0; i < 4; ++i) {
      lastServerSwitchStates[i] = _[i];
    }
    free(_);
  }
}

void loop()
{
  // bool freshServerSwitchState = getValueFromWifi();
  bool freshServerSwitchStates[4] = {false, false, false, false};
  // bool freshServerSwitchStates = getValuesFromWifi();
  {
    bool* _ = getValuesFromWifi();
    for (int i = 0; i < 4; ++i) {
      freshServerSwitchStates[i] = _[i];
    }
    free(_);
  }

  // bool stateNotChanged = freshServerSwitchState == lastServerSwitchState;
  bool stateNotChanged = memcpy(freshServerSwitchStates, lastServerSwitchStates, 4);

  if (stateNotChanged)
  {
    delay(500); // prevent too frequent polling when can't find server
    return; // do nothing
  }

  // lastServerSwitchState = freshServerSwitchState; // update board value
  memcpy(lastServerSwitchStates, freshServerSwitchStates, 4); // update board value

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
  bool relayValues_sane[4] = {false, false, false, false};
  for(int i = 0; i < 4; i++) relayValues_sane[i] = !freshServerSwitchStates[i];
  // since relay is active LOW, but I find active HIGH to be more comfortable

  // digitalWrite(RELAY_PIN, relayValue_sane);           // true means ON, false means OFF (sane)
  for(int i = 0; i < 4; i++) digitalWrite(RELAY_PINS[i], relayValues_sane[i]);

  // digitalWrite(LED_BUILTIN, !digitalRead(RELAY_PIN)); // update as per relay value
  // has only one LED, ignore

  delay(1000);                                        // poll every 5 second
}
