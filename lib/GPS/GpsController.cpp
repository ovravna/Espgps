#include "GpsController.h"



bool GpsController::begin() {
    
    GPS.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); 

    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
    // the parser doesn't care about other sentences at this time
    // Set the update rate
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ); // 1 Hz update rate
    return true;

}


bool GpsController::handle() {
    GPS.read();
    // if (c) Serial.print(c);


    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
        // Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        return false; // we can fail to parse a sentence in which case we should just wait for another
    }
    // if millis() or timer wraps around, we'll just reset it
    

    // if (timer > millis()) timer = millis();
        
    // // approximately every 2 seconds or so, print out the current stats
    // if (millis() - timer > 2000) {
    //     timer = millis(); // reset the timer
    //     Serial.print("\nTime: ");
    //     Serial.print(GPS.hour, DEC); Serial.print(':');
    //     Serial.print(GPS.minute, DEC); Serial.print(':');
    //     Serial.print(GPS.seconds, DEC); Serial.print('.');
    //     Serial.println(GPS.milliseconds);
    //     Serial.print("Date: ");
    //     Serial.print(GPS.day, DEC); Serial.print('/');
    //     Serial.print(GPS.month, DEC); Serial.print("/20");
    //     Serial.println(GPS.year, DEC);
    //     Serial.print("Fix: "); Serial.print((int)GPS.fix);
    //     Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    //     if (GPS.fix) {
    //         Serial.print("Location: ");
    //         Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
    //         Serial.print(", ");
    //         Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    //         Serial.print("Speed (knots): "); Serial.println(GPS.speed);
    //         Serial.print("Angle: "); Serial.println(GPS.angle);
    //         Serial.print("Altitude: "); Serial.println(GPS.altitude);
    //         Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    //     }
    // }

    return true; 
}