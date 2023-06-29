/**
What: Arduino + Relay control + AC appliance connected to mains

Circuits
1. Load circuit - Make a circuit of fan + relay + mains (home socket)
2. Relay to Load circuit - Now, break circuit #1 from some place and connect the ends into the relay (in the open and middle ends, leave the closed boxed end as-is)
3. Arduino to relay
  1. Connect VCC of relay to 5 or 3.3 V pin of Arduino.
  2. Connect GND of relay to a GND pin of Arduino.
  3. Connect relay control pin to a number pin (say 8) of Arduino.

Code: setting numbered pin to LOW means the relay is ON (circuit completes), otherwise OFF. (yeah, low means ON, weird)
*/
int relay_pin = 8;

void setup() {
  pinMode(relay_pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  // turn OFF the relay
  // and wait
  digitalWrite(relay_pin, HIGH);
  digitalWrite(LED_BUILTIN, !digitalRead(relay_pin));
  delay(1000);


  // turn ON the relay
  // and wait
  digitalWrite(relay_pin, LOW);
  digitalWrite(LED_BUILTIN, !digitalRead(relay_pin));
  delay(1000);
}