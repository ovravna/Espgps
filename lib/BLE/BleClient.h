#ifndef E_BLE_C_H
#define E_BLE_C_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <IDevice.h>

class BleClient: public IDevice {
    const uint8_t notificationOff[2] = {0x0, 0x0};
    const uint8_t notificationOn[2] = {0x1, 0x0};
    const uint8_t indicationOn[2] = {0x2, 0x0};


    std::map<String, BLERemoteCharacteristic *> characteristics{};

    const uint32_t SCAN_DURATION = 30;

    bool doConnect = false;
    static bool validConnection;


    BLEClient *pClient = nullptr;
    BLEAddress *serverAddress = nullptr;
    BLERemoteService *pRemoteService = nullptr;



    static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                               uint8_t *pData,
                               size_t length,
                               bool isNotify);
   

    void init();

    bool connect(BLEAddress serverAddress);

    bool addCharacteristic(const char *uuid);

    friend void tryConnect(BleClient *client, BLEAddress *serverAddress);

    friend class AdvertisedCallback; 
    
public:

    bool begin();

    bool handle();

};


 class AdvertisedCallback: public BLEAdvertisedDeviceCallbacks {
    BleClient *parent;


public:
    explicit AdvertisedCallback(BleClient *parent): parent(parent) {
    };

    void onResult(BLEAdvertisedDevice advertisedDevice) override;

};

#endif /* E_BLE_C_H */