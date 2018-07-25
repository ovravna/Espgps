#include <Arduino.h>
#include <IridiumSBD.h>
#include "../IDevice.h"


class E_Iridium: public IDevice {
    HardwareSerial      iridiumSerial;
    IridiumSBD          modem;
    int                 RX_PIN, TX_PIN;

public:
    explicit E_Iridium(HardwareSerial *hwSerial, int rxPin, int txPin): 
            iridiumSerial(*hwSerial), 
            modem(*hwSerial),
            RX_PIN(rxPin),
            TX_PIN(txPin) {}

    bool begin() override;

    bool handle() override;


};
