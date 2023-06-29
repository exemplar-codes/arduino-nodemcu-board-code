// poll the server and switch relay as needed
// update the external PIN to the relay's value (LED should be ON if relay is ON)

#define RELAY_PIN 5 // D1 pin on board

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop()
{
  // turn OFF the relay
  // and wait
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_BUILTIN, !digitalRead(RELAY_PIN));
  delay(1000);

  // turn ON the relay
  // and wait
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_BUILTIN, !digitalRead(RELAY_PIN));
  delay(1000);
}
