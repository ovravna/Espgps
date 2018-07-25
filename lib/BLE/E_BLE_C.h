#ifndef E_BLE_C_H
#define E_BLE_C_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <IDevice.h>

class E_BLE_C: public IDevice {
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

    friend void tryConnect(E_BLE_C *client, BLEAddress *serverAddress);

    friend class AdvertisedCallback; 
    
public:

    bool begin();

    bool handle();

};


 class AdvertisedCallback: public BLEAdvertisedDeviceCallbacks {
    E_BLE_C *parent;


public:
    explicit AdvertisedCallback(E_BLE_C *parent): parent(parent) {
    };

    void onResult(BLEAdvertisedDevice advertisedDevice) override;

};

#endif /* E_BLE_C_H */