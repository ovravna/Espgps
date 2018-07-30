#include <Arduino.h>
#include "utils.h"
#include <E_BLE_C.h>
#include <UUIDs.h>


E_BLE_C ble;



void setup() {
  Serial.begin(19200);
  ble.begin();
  
}


     
void loop() {
  ble.handle();
  synchronize(500);
} 