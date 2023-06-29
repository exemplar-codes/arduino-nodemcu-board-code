#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

 
const char* ssid = "Ahmar";
const char* password = "808@4443022";
 
void setup () {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
 
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
WiFiClient wifiClient;
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    // http.begin(wifiClient, "http://jsonplaceholder.typicode.com/users/1");  //Specify request destination
    http.begin(wifiClient, "https://api.github.com/users/sanjarcode");  //Specify request destination
    int httpCode = http.GET();                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
 
    }
 
    http.end();   //Close connection
 
  }
 
  delay(30000);    //Send a request every 30 seconds
}