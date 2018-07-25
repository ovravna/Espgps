#include "E_Iridium.h"



bool E_Iridium::begin() {

    int signalQuality = -1;
    int err;
    iridiumSerial.begin(19200, SERIAL_8N1, RX_PIN, TX_PIN);

    modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);

    // Begin satellite modem operation
    Serial.println("Starting modem...");
    err = modem.begin();
    if (err != ISBD_SUCCESS)
    {
        Serial.print("Begin failed: error ");
        Serial.println(err);
        if (err == ISBD_NO_MODEM_DETECTED)
        Serial.println("No modem detected: check wiring.");
        return false;
    }

    // Example: Print the firmware revision
    char version[12];
    err = modem.getFirmwareVersion(version, sizeof(version));
    if (err != ISBD_SUCCESS)
    {
        Serial.print("FirmwareVersion failed: error ");
        Serial.println(err);
    }
    Serial.print("Firmware Version is ");
    Serial.print(version);
    Serial.println(".");

    // Example: Test the signal quality.
    // This returns a number between 0 and 5.
    // 2 or better is preferred.
    err = modem.getSignalQuality(signalQuality);
    if (err != ISBD_SUCCESS)
    {
        Serial.print("SignalQuality failed: error ");
        Serial.println(err);
    }

    Serial.print("On a scale of 0 to 5, signal quality is currently ");
    Serial.print(signalQuality);
    Serial.println(".");

    // Send the message
    Serial.print("Trying to send the message.  This might take several minutes.\r\n");
    err = modem.sendSBDText("Hello, world!");
    if (err != ISBD_SUCCESS)
    {
        Serial.print("sendSBDText failed: error ");
        Serial.println(err);
        if (err == ISBD_SENDRECEIVE_TIMEOUT)
        Serial.println("Try again with a better view of the sky.");
    }


    return true;
}


bool E_Iridium::handle() {


    

    return true;
}