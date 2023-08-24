#include <stdio.h>
#include <stdlib.h>
// https://chat.openai.com/share/d9ea5770-9f2d-4b55-9d78-f0a244937b1a
// Function pointer type for mapping and filtering functions
typedef void* (*MappingFunction)(void*);
typedef int (*FilterFunction)(void*);

// Map function: applies the provided function to each element of the array
void** map(void* arr[], int length, MappingFunction mapper) {
    void** result = malloc(length * sizeof(void*));
    for (int i = 0; i < length; i++) {
        result[i] = mapper(arr[i]);
    }
    return result;
}

// Filter function: filters the array based on the provided predicate function
void** filter(void* arr[], int length, FilterFunction predicate, int* resultLength) {
    void** result = malloc(length * sizeof(void*));
    *resultLength = 0;
    for (int i = 0; i < length; i++) {
        if (predicate(arr[i])) {
            result[(*resultLength)++] = arr[i];
        }
    }
    return result;
}

// Mapping function example: square the number
void* square(void* item) {
    int* num = (int*)item;
    int* result = malloc(sizeof(int));
    *result = (*num) * (*num);
    return result;
}

// Filtering function example: check if the number is even
int isEven(void* item) {
    int* num = (int*)item;
    return (*num) % 2 == 0;
}

int main() {
    int num1 = 1, num2 = 2, num3 = 3, num4 = 4, num5 = 5;
    void* arr[] = {&num1, &num2, &num3, &num4, &num5};
    int length = sizeof(arr) / sizeof(arr[0]);

    void** squared = map(arr, length, square);
    int squaredLength = length;

    void** evenNumbers = filter(arr, length, isEven, &squaredLength);

    printf("Original array: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", *(int*)arr[i]);
    }
    printf("\n");

    printf("Squared array: ");
    for (int i = 0; i < squaredLength; i++) {
        printf("%d ", *(int*)squared[i]);
        free(squared[i]); // Free memory allocated in square function
    }
    printf("\n");

    printf("Even numbers array: ");
    for (int i = 0; i < squaredLength; i++) {
        printf("%d ", *(int*)evenNumbers[i]);
    }
    printf("\n");

    free(squared);
    free(evenNumbers);

    return 0;
}

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
    Serial.print("Connecting..");
  }
}

char* serverUrl[100];

void cycleThroughURLs(void (*CallbackFunction)(const char *), const char *baseURL, const char *port, int initialIP, int numVariations) {
    if (baseURL == NULL) baseURL = "http://192.168.0.";
    if (port == NULL) port = ":3000";
    if (initialIP == 0) initialIP = 100;
    if (numVariations == 0) numVariations = 11;

    for (int i = 0; i < numVariations; i++) {
        char generatedURL[100]; // Adjust the size as needed
        sprintf(generatedURL, "%s%d%s", baseURL, initialIP + i, port);

        if (CallbackFunction != NULL) {
            CallbackFunction(generatedURL);
        } else {
            printf("%s\n", generatedURL);
        }
    }
}

bool checkIfURLIsLive(const char *url) {
}

void setFoundServerURL(const char *url) {
    printf("Checking URL: %s\n", url);
    // add http code check
    bool foundServer = 0;
    if(foundServer)
        printf("Found server URL: %s\n", serverUrl);
}


void findServerInNetwork() {
  WiFiClient wifiClient;

    HTTPClient http; // Declare an object of class HTTPClient

    http.begin(wifiClient, serverUrl)

    cycleThroughURLs(setFoundServerURL, NULL, NULL, 0, 0);
}

// serverUrl

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
