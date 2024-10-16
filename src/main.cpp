#include <Arduino.h>
// put function declarations here:

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
 digitalWrite(LED_BUILTIN,HIGH);
 delay(1000);
 digitalWrite(LED_BUILTIN,LOW);
 delay(1000);

}
