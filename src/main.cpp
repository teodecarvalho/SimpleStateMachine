#include <Arduino.h>
#include "device.h"

#define pinLed 13

Listener listener;
Device *device = new Device(new StateLedOn);


void setup() {
  Serial.begin(9600);
  Serial.println("Started");
  pinMode(pinLed, OUTPUT);
}

void loop() {
    device->update();
}