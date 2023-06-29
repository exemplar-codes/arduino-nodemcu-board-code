// NodeMCU has fucked up label and GPIO numbering
// D1 pin's value is 5

#define EXTERNAL_LED 5

void setup() {
  pinMode(EXTERNAL_LED, OUTPUT);  // LED pin as output.
}

void loop() {
  digitalWrite(EXTERNAL_LED, LOW);   // turn the LED off.(Note that LOW is the voltage level but actually
                           //the LED is on; this is because it is acive low on the ESP8266.
  delay(500);              // wait for 1 second.
  digitalWrite(EXTERNAL_LED, HIGH);  // turn the LED on.
  delay(500);              // wait for 1 second.
}
