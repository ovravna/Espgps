#ifndef E_BLE_S_H
#define E_BLE_S_H

#include <BLEDevice.h>
#include <Arduino.h>
#include <IDevice.h>
#include <Project.h>

class BleServer: public IDevice {
    std::map<std::string, BLECharacteristic *> characteristics{};
    BLEService *pService = nullptr;
    BLEServer *pServer = nullptr;
    bool bleConnected = false;


    friend class CharacteristicCallback;
    friend class ConnectionCallback;

public:

    bool begin() override;

    bool handle() override;

    bool setCharacteristic(std::string UUID, std::string value);




};

class ConnectionCallback: public BLEServerCallbacks {
    BleServer *parent;
public:
    explicit ConnectionCallback(BleServer *parent): parent(parent)  {
        if (DEBUG) Serial.println("Connection callback created!");
    }

    void onConnect(BLEServer *pServer) override;
    void onDisconnect(BLEServer *pServer) override;
};

class CharacteristicCallback: public BLECharacteristicCallbacks {
    const char *uuid;
public:
    explicit CharacteristicCallback(const char *uuid): uuid(uuid) {};

    void onRead(BLECharacteristic *pCharacteristic) override;

    void onWrite(BLECharacteristic *pCharacteristic) override;


};

#endif /* E_BLE_S_H */