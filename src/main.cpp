#include <Arduino.h>
#include "GPS/E_GPS.h"
#include "BLE/E_BLE_C.h"
#include "BLE/E_BLE_S.h"
#include "Iridium/E_Iridium.h"
#include <Print.h>
#include "UUIDs.h"
#include "utils.h"


HardwareSerial GPSSerial(1);

// E_GPS G(&GPSSerial, 32, 33);

// E_BLE_C C;

// E_BLE_S S;

// E_Iridium I(&GPSSerial, 32, 33);

IridiumSBD modem(GPSSerial);


void setup() {
  int signalQuality = -1;

  // S.begin();
  Serial.begin(19200);
  while(!Serial);
  // while (!Serial);
  Serial.println("Serial started...");
  GPSSerial.begin(19200, SERIAL_8N1, 32, 33);

  // Serial.println("IRIDIUM Serial started...");


  // // Setup the Iridium modem
  // modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);
  // Serial.println("IRIDIUM set power profile...");

  // if (modem.begin() != ISBD_SUCCESS)
  // {
  //   Serial.println("Couldn't begin modem operations.");
  // }

  // Serial.println("IRIDIUM modem has begun...");


  // // Check the signal quality (optional)
  // int err = modem.getSignalQuality(signalQuality);
  // if (err != 0)
  // {
  //   Serial.print("SignalQuality failed: error ");
  //   Serial.println(err);
  // }

  // Serial.print("Signal quality is ");
  // Serial.println(signalQuality);

  // Serial.println("Enter commands terminated by Carriage Return ('\\r'):");

  // G.begin();
  // delay(500);
  // C.begin();
  // S.begin();
  // I.begin();
  
}

uint32_t timer = millis();
     
void loop() {

  
  // if (Serial.available())
  //   GPSSerial.write(Serial.read());
  // if (GPSSerial.available())
  //   Serial.write(GPSSerial.read());
  

  if (GPSSerial.available()) {
    Serial.write(GPSSerial.read());

  }


  // // G.handle();


  if (timer > millis()) timer = millis();
        
  //   // approximately every 2 seconds or so, print out the current stats



  if (millis() - timer > 2000) {
    const char* at = "AT&K?\r\n";
    GPSSerial.write(at);
    Serial.write(at);
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

    
  //     Serial.print("\nTime: ");
  //     Serial.print(G.GPS.hour, DEC); Serial.print(':');
  //     Serial.print(G.GPS.minute, DEC); Serial.print(':');
  //     Serial.print(G.GPS.seconds, DEC); Serial.print('.');
  //     Serial.println(G.GPS.milliseconds);
  //     Serial.print("Date: ");
  //     Serial.print(G.GPS.day, DEC); Serial.print('/');
  //     Serial.print(G.GPS.month, DEC); Serial.print("/20");
  //     Serial.println(G.GPS.year, DEC);
  //     Serial.print("Fix: "); Serial.print((int)G.GPS.fix);
  //     Serial.print(" quality: "); Serial.println((int)G.GPS.fixquality);
  //     if (G.GPS.fix) {
  //         Serial.print("Location: ");
  //         Serial.print(G.GPS.latitude_fixed); 
  //         Serial.print(", ");
  //         Serial.print(G.GPS.longitude_fixed);
  //         Serial.print("\nLocation (in degrees, works with Google Maps): ");
  //         Serial.print(G.GPS.latitudeDegrees, 4);
  //         Serial.print(", "); 
  //         Serial.println(G.GPS.longitudeDegrees, 4);
  //         Serial.print("Speed (knots): "); Serial.println(G.GPS.speed);
  //         Serial.print("Angle: "); Serial.println(G.GPS.angle);
  //         Serial.print("Altitude: "); Serial.println(G.GPS.altitude);
  //         Serial.print("Satellites: "); Serial.println((int)G.GPS.satellites);
  //     }
  // }



}


/*
65844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436765844367838113106584436783816584436783811310658400000000000000000000000000000000000000000000000000000000000000000000000013658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310106584436783811310658443678310658443678381131065844367831065844367838113106584436783106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658410658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584431006584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310658443678381131065844367838113106584436783811310.␍␊

*/