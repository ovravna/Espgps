#include <Arduino.h>
#include <E_GPS.h>
#include <E_BLE_C.h>
#include <E_BLE_S.h>
#include <IridiumController.h>
#include <Print.h>
#include <UUIDs.h>
#include "utils.h"


HardwareSerial IridiumSerial(1);
HardwareSerial GPSSerial(2);

E_GPS gps(&GPSSerial, 25, 26);

// E_BLE_C C;

E_BLE_S bleServer;

// E_Iridium I(&GPSSerial, 32, 33);


IridiumController iridium(IridiumSerial, 32, 33, 35);
void callback(String command, String response) {
   Serial.println("Response is: " + response);
}

void setup() {
  int signalQuality = -1;

  // S.begin();
  Serial.begin(19200);
  while(!Serial);
  // while (!Serial);
  Serial.println("Serial started...");
  iridium.setResponseCallcack(callback);

  // G.begin();
  // delay(500);
  // C.begin();

  gps.begin();
  bleServer.begin();
  iridium.begin();
  
}

uint32_t timer = millis();
     
void loop() {
  iridium.handle();
  gps.handle();

  bleServer.handle();

  
  
  // if (Serial.available())
  //   GPSSerial.write(Serial.read());
  // if (GPSSerial.available())
  //   Serial.write(GPSSerial.read());
  

  

  // // G.handle();


  if (timer > millis()) timer = millis();
        
  //   // approximately every 2 seconds or so, print out the current stats



  if (millis() - timer > 2000) {


    timer = millis(); // reset the timer
  }
      
      // std::string latlon = "";
  //     char buf[10];


  //     sprintf(buf, "%.4f", G.GPS.latitude);
  //     latlon.append(buf);
  //     latlon += G.GPS.lat;
  //     latlon += ',';
  //     sprintf(buf, "%.4f", G.GPS.longitude);
  //     latlon.append(buf);
  //     latlon += G.GPS.lon;
    
      
    
  //     S.setCharacteristic(COORDINATES_CHAR_UUID, 
  //         std::string(Coordinate { G.GPS.latitude_fixed, G.GPS.longitude_fixed }.b));

  //     S.handle();



}


/*
65844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436765844367838113106584436783816584436783811310658400000000000000000000000000000000000000000000000000000000000000000000000013658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310106584436783811310658443678310658443678381131065844367831065844367838113106584436783106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658410658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584431006584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310.␍␊

*/