#include <Arduino.h>
#include <IridiumController.h>
#include <E_GPS.h>
#include "utils.h"


#define TEST_LED 14
#include <Switch.h>


HardwareSerial iridiumSerial(1);
HardwareSerial gpsSerial(2);


E_GPS gps(&gpsSerial, 25, 26);
IridiumController iridium(iridiumSerial, 32, 33, 35);
Switch sunamiController;

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

  if (DEBUG) Serial.println("SBD received: " + message);



}

void callback(String command, String response) {
  if (DEBUG) Serial.println("Response is: " + response);
}

void setup() {

 

	// iridium.addCallcack(callback);

	Serial.begin(19200);
	if (DEBUG) Serial.println("Started!");

  //Set callbacks
  iridium.setResponseCallcack(callback);
  iridium.setSBDCallcack(receiveSBDcallback);

  //begin
  iridium.begin();
  gps.begin();
	// iridium.pushCommand("AT+CSQ", "OK");

}

unsigned long t2;
unsigned long t;
unsigned long pdiff;

void synchronize(unsigned long period = 1000) {

	pdiff = (millis() - t);
	if (pdiff < period) delay(period - pdiff);
	t = millis();
}

void loop() {
    iridium.handle();
    gps.handle();


    if (Serial.available()) {    
        String s = readLine();
        if (s == "GO") {
          Serial.println("Sending message!");
          iridium.sendSBDtext("NINJA!");
        } 

        if (s == "I") {
          iridium.pushCommand("AT+SBDIX", "OK");
        }
        if (s == "IX") {
          iridium.pushCommand("AT+SBDIX", "OK");
        }

        if (s == "G") {
          Serial.print("\nTime: ");
          Serial.print(gps.GPS.hour, DEC); Serial.print(':');
          Serial.print(gps.GPS.minute, DEC); Serial.print(':');
          Serial.print(gps.GPS.seconds, DEC); Serial.print('.');
          Serial.println(gps.GPS.milliseconds);
          Serial.print("Date: ");
          Serial.print(gps.GPS.day, DEC); Serial.print('/');
          Serial.print(gps.GPS.month, DEC); Serial.print("/20");
          Serial.println(gps.GPS.year, DEC);
          Serial.print("Fix: "); Serial.print((int)gps.GPS.fix);
          Serial.print(" quality: "); Serial.println((int)gps.GPS.fixquality); 
          if (gps.GPS.fix) {
            Serial.print("Location: ");
            Serial.print(gps.GPS.latitude, 4); Serial.print(gps.GPS.lat);
            Serial.print(", "); 
            Serial.print(gps.GPS.longitude, 4); Serial.println(gps.GPS.lon);
            Serial.print("Location (in degrees, works with Google Maps): ");
            Serial.print(gps.GPS.latitudeDegrees, 4);
            Serial.print(", "); 
            Serial.println(gps.GPS.longitudeDegrees, 4);
            
            Serial.print("Speed (knots): "); Serial.println(gps.GPS.speed);
            Serial.print("Angle: "); Serial.println(gps.GPS.angle);
            Serial.print("Altitude: "); Serial.println(gps.GPS.altitude);
            Serial.print("Satellites: "); Serial.println((int)gps.GPS.satellites);
          }

        }

        if (s == "GPS") {

          String s = String(gps.GPS.latitudeDegrees, 4)
          + String(gps.GPS.lat) + ", " 
          + String(gps.GPS.longitudeDegrees, 4) 
          + String(gps.GPS.lon); 

          iridium.sendSBDtext(s);

        }
    
        if (s == "CSQ") {
          iridium.pushCommand("AT+CSQ", "OK");
        }

        if (s == "D0") {
          iridium.pushCommand("AT+SBDD0", NUM);
        }
  
        if (s == "RT") {
          iridium.pushCommand("AT+SBDRT", "OK");
        }

        if (s == "L") {
          Serial.println(digitalRead(TEST_LED));
        }

        if (s == "L+") {
          digitalWrite(TEST_LED, HIGH);
        }

        if (s == "L-") {
          digitalWrite(TEST_LED, LOW);
        }

        if (s == "STAT") {
          Serial.println("MO_status: "+String(iridium.status.MO_status));
          Serial.println("MOMSN: "+String(iridium.status.MOMSN));
          Serial.println("MT_length: "+String(iridium.status.MT_length));
          Serial.println("MTMSN: "+String(iridium.status.MTMSN));
          Serial.println("MT_length: "+String(iridium.status.MT_length));
          Serial.println("MT_queued: "+String(iridium.status.MT_queued));
          Serial.println("SignalQuality: "+String(iridium.status.SignalQuality));
          Serial.println("REG: "+String(iridium.status.REG));
        }

        if (s.startsWith(":")) {
          int i = s.indexOf(";");
          if (i != -1)
            iridium.pushCommand(s.substring(1, i), s.substring(i + 1));
          else 
            iridium.pushCommand(s.substring(1, i), "OK");

        }
    }

    // iridium.command("AT+CSQ", "OK");

    // Serial.print(".");
	synchronize(50);
}