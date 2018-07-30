
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <IDevice.h>


class GpsContoller : public IDevice {
    HardwareSerial      GPSSerial;
    int                 RX_PIN;
    int                 TX_PIN;

    // uint32_t timer =    millis();



public:
    Adafruit_GPS        GPS;

    explicit GpsContoller(HardwareSerial *hwSerial, int rxPin, int txPin) : 
            GPSSerial(*hwSerial), 
            RX_PIN(rxPin),
            TX_PIN(txPin),
            GPS(hwSerial) { }


    bool begin() override;

    bool handle() override;


};