// NodeMCU has two controllable onboard LEDS

// Onboard LED number 1 - at far side of microusb port is called `LED_BUILTIN` (predefined alias, value is `2`)
// Onboard LED number 2 - near the microusb port is called `D0` (predefined alias), value is `16`)

// Note: the LED are

// #define LED_BUILTIN D0  // Led in NodeMCU at pin GPIO16 (D0).
void setup() {
  pinMode(D0, OUTPUT);  // LED pin as output.
}

void loop() {
  digitalWrite(D0, LOW);   // turn the LED off.(Note that LOW is the voltage level but actually
                           //the LED is on; this is because it is acive low on the ESP8266.
  delay(500);              // wait for 1 second.
  digitalWrite(D0, HIGH);  // turn the LED on.
  delay(500);              // wait for 1 second.
}