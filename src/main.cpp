#include <Arduino.h>
#include <GpsController.h>
#include <BleClient.h>
#include <BleServer.h>
#include <IridiumController.h>
#include <Print.h>
#include <UUIDs.h>
#include "utils.h"
#include <Switch.h>

#ifdef SERVER
BleServer ble;
#else 
BleClient ble;
#endif






#ifdef IRIDIUM_DEVICE
HardwareSerial iridiumSerial(1);

IridiumController iridium(iridiumSerial, 32, 33, 35);
Switch sunamiController;
#endif


#ifdef GPS_DEVICE
HardwareSerial gpsSerial(2);

GpsController gps(&gpsSerial, 25, 26);
#endif


#ifdef IRIDIUM_DEVICE

void receiveSBDcallback(String message) {

  
  //state 0
  if (message.startsWith("s0")) {
    //turn off led
    //sendSBDtext( state of led now )
    bool state = sunamiController.turnOff();

    
    iridium.sendSBDtext(String(state));
  }

  //state 1
  if (message.startsWith("s1")) {
    //turn on led
    //sendSBDtext( state of led now )


    bool state = sunamiController.turnOff();
    iridium.sendSBDtext(String(state));
  }

  //Query state
  if (message.startsWith("qs")) {
    //sendSBDtext( state of led now )    
    iridium.sendSBDtext(String(sunamiController.getState()));

  }


#ifdef GPS_DEVICE
  //Query location
  if (message.startsWith("ql")) {
    
    String s = String(gps.GPS.latitudeDegrees, 4)
          + String(gps.GPS.lat) + ", " 
          + String(gps.GPS.longitudeDegrees, 4) 
          + String(gps.GPS.lon); 

    iridium.sendSBDtext(s);
  }

  //Query coordinates
  if (message.startsWith("qc")) {

    char* s = IridiumController::bytify(gps.GPS.latitude_fixed, gps.GPS.longitude_fixed);
    iridium.sendSBDtext(s);
    
  }
#endif


  if (DEBUG) Serial.println("SBD received: " + message);

}

void callback(String command, String response) {
  if (DEBUG) Serial.println("Response is: " + response);
}
#endif


void setup() {

  Serial.begin(19200);
  if (DEBUG) Serial.println("Serial started...");

  #ifdef IRIDIUM_DEVICE
  iridium.setResponseCallback(callback);
  iridium.setSBDCallback(receiveSBDcallback);
  iridium.begin();
  #endif

  #ifdef GPS_DEVICE
  gps.begin();
  #endif

  // iridium.setResponseCallback(callback);


  // gps.begin();
  ble.begin();
  // iridium.begin();
  
}


void loop() {
  #ifdef IRIDIUM_DEVICE
  iridium.handle();
  #endif

  #ifdef GPS_DEVICE
  gps.handle();
  #endif

  // ble.setCharacteristic()
  
  ble.handle();

  
  synchronize(500);
} 
