#include <Arduino.h>
#include <E_GPS.h>
#include <E_BLE_C.h>
#include <E_BLE_S.h>
#include <IridiumController.h>
#include <Print.h>
#include <UUIDs.h>
#include "utils.h"


// HardwareSerial IridiumSerial(1);
// HardwareSerial GPSSerial(2);

// E_GPS gps(&GPSSerial, 25, 26);

// E_BLE_C C;

#ifdef SERVER
E_BLE_S ble;
#else 
E_BLE_C ble;
#endif

// E_Iridium I(&GPSSerial, 32, 33);


// IridiumController iridium(IridiumSerial, 32, 33, 35);
void callback(String command, String response) {
   if (DEBUG) Serial.println("Response is: " + response);
}

void setup() {

  Serial.begin(19200);
  if (DEBUG) Serial.println("Serial started...");

  // iridium.setResponseCallcack(callback);


  // gps.begin();
  ble.begin();
  // iridium.begin();
  
}

uint32_t timer = millis();
     
void loop() {
  // iridium.handle();
  // gps.handle();

  // ble.setCharacteristic()
  
    ble.handle();

  
  
  // if (Serial.available())
  //   GPSSerial.write(Serial.read());
  // if (GPSSerial.available())
  //   Serial.write(GPSSerial.read());
  

  

  // // G.handle();


  if (timer > millis()) timer = millis();
        
  //   // approximately every 2 seconds or so, print out the current stats



  if (millis() - timer > 500) {

    timer = millis(); // reset the timer
  }

      
    
  //     S.setCharacteristic(COORDINATES_CHAR_UUID, 
  //         std::string(Coordinate { G.GPS.latitude_fixed, G.GPS.longitude_fixed }.b));

  //     S.handle();


  
}
