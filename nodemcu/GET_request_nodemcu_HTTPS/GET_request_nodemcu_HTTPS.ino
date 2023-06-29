#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>  // Use WiFiClientSecure for HTTPS requests

const char* ssid = "yourSSID";
const char* password = "yourPassword";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure wifiClient;  // Use WiFiClientSecure for HTTPS requests

    HTTPClient http;
    http.begin(wifiClient, "https://api.github.com/users/sanjarcode");

    // Trust the SSL certificate (needed for HTTPS requests)
    wifiClient.setInsecure();

    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }

    http.end();
  }

  delay(30000);
}
