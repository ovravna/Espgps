#include "BleServer.h"
#include <Project.h>
#include <BLE2902.h>


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

    if (DEBUG) Serial.println("Read BLE");

}

void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    if (DEBUG) Serial.println("Write BLE");

}

bool BleServer::setCharacteristic(std::string uuid, std::string value) {

    characteristics[uuid]->setValue(value);

    return true;
}

bool BleServer::begin() {

    BLEDevice::init(SERVER_NAME);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ConnectionCallback(this));

    //BLEUUID aux("91bad492-b950-4226-aa2b-4ede9fa42f59");
    
    pService = pServer->createService(BLEUUID(SERVICE_UUID));

    // characteristics[CHARACTERISTIC_UUID] = pService->createCharacteristic(
    //         CHARACTERISTIC_UUID,
    //         BLECharacteristic::PROPERTY_READ |
    //         BLECharacteristic::PROPERTY_WRITE
    // );
    // characteristics[CHARACTERISTIC_UUID]->setCallbacks(new CharacteristicCallback(CHARACTERISTIC_UUID));
    // characteristics[CHARACTERISTIC_UUID]->setValue("Hello World");
    BLEDescriptor textDescriptor(BLEUUID((uint16_t)0x2901));

    textDescriptor.setValue("Power!");


    characteristics[POWER_CHAR_UUID] = pService->createCharacteristic(
            POWER_CHAR_UUID,
            BLECharacteristic::PROPERTY_NOTIFY
    );

    characteristics[POWER_CHAR_UUID]->addDescriptor(&textDescriptor);
    characteristics[POWER_CHAR_UUID]->addDescriptor(new BLE2902());
    characteristics[POWER_CHAR_UUID]->setValue("OK");

    // characteristics[COORDINATES_CHAR_UUID] = pService->createCharacteristic(
    //         COORDINATES_CHAR_UUID,
    //         BLECharacteristic::PROPERTY_READ
    // );

    // characteristics[COORDINATES_CHAR_UUID]->setValue("N/A");

    // characteristics[CONNECTION_CHAR_UUID] = pService->createCharacteristic(
    //         CONNECTION_CHAR_UUID,
    //         BLECharacteristic::PROPERTY_WRITE
    // );

    pServer->getAdvertising()->addServiceUUID(BLEUUID(SERVICE_UUID));

    pService->start();
    pServer->getAdvertising()->start();
    if (DEBUG) Serial.println("Started advertising...");

    return true;

}

bool BleServer::handle() {

    // if (digitalRead(18)) {
    //     characteristics[POWER_CHAR_UUID]->setValue("ERR");
    // } else {
    //     characteristics[POWER_CHAR_UUID]->setValue("OK");

    // }

    characteristics[POWER_CHAR_UUID]->notify();
    

    return true;
}
