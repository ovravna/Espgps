
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <IDevice.h>


class GpsController : public IDevice {
    HardwareSerial      GPSSerial;
    int                 RX_PIN;
    int                 TX_PIN;

    // uint32_t timer =    millis();



public:
    Adafruit_GPS        GPS;

    explicit GpsController(HardwareSerial *hwSerial, int rxPin, int txPin) : 
            GPSSerial(*hwSerial), 
            RX_PIN(rxPin),
            TX_PIN(txPin),
            GPS(hwSerial) { }


    bool begin() override;

    bool handle() override;


};