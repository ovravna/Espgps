#include "E_BLE_S.h"
#include <UUIDs.h>



void ConnectionCallback::onConnect(BLEServer *pServer) {
    if (DEBUG) {
        Serial.print("Connected!! Clients connected: ");
        Serial.println(pServer->getConnectedCount() + 1);

    }

    parent->bleConnected = true;
    pServer->getAdvertising()->start();

}

void ConnectionCallback::onDisconnect(BLEServer *pServer) {
    if (DEBUG) {
        Serial.print("Disconnected! Clients connected: ");
        Serial.println(pServer->getConnectedCount());
    }

    parent->bleConnected = pServer->getConnectedCount() != 0;

}

void CharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {

}

void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {

}

bool E_BLE_S::setCharacteristic(std::string uuid, std::string value) {

    characteristics[uuid]->setValue(value);

    return true;
}

bool E_BLE_S::begin() {

    BLEDevice::init(SERVER_NAME);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ConnectionCallback(this));

    pService = pServer->createService(SERVICE_UUID);

    characteristics[CHARACTERISTIC_UUID] = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE
    );
    characteristics[CHARACTERISTIC_UUID]->setCallbacks(new CharacteristicCallback(CHARACTERISTIC_UUID));
    characteristics[CHARACTERISTIC_UUID]->setValue("Hello World");


    characteristics[POWER_CHAR_UUID] = pService->createCharacteristic(
            POWER_CHAR_UUID,
            BLECharacteristic::PROPERTY_NOTIFY
    );

    characteristics[POWER_CHAR_UUID]->setValue("OK");

    characteristics[COORDINATES_CHAR_UUID] = pService->createCharacteristic(
            COORDINATES_CHAR_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY
    );

    characteristics[COORDINATES_CHAR_UUID]->setValue("N/A");

    characteristics[CONNECTION_CHAR_UUID] = pService->createCharacteristic(
            CONNECTION_CHAR_UUID,
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE
    );


    pService->start();
    pServer->getAdvertising()->start();
    if (DEBUG) Serial.println("Started advertising...");

    return true;

}

bool E_BLE_S::handle() {

    // if (digitalRead(18)) {
    //     characteristics[POWER_CHAR_UUID]->setValue("ERR");
    // } else {
    //     characteristics[POWER_CHAR_UUID]->setValue("OK");

    // }

    characteristics[POWER_CHAR_UUID]->notify();

    return true;
}
