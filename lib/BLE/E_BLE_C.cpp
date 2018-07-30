#include "E_BLE_C.h"
#include <UUIDs.h>
#include <Log.h>




void AdvertisedCallback::onResult(BLEAdvertisedDevice advertisedDevice) {

    if (DEBUG) {
        Serial.print("Addr: ");
        Serial.print(advertisedDevice.getAddress().toString().c_str());
        if (advertisedDevice.haveName()) {
            Serial.print("| Name: " + String(advertisedDevice.getName().c_str()));

        }
        Serial.println();        


    }

    if (advertisedDevice.haveName() && advertisedDevice.getName() == "ESP32_SERVER") {

        advertisedDevice.getScan()->stop();


        parent->serverAddress = new BLEAddress(advertisedDevice.getAddress());
        parent->doConnect = true;

    }


}

bool E_BLE_C::connect(BLEAddress serverAddress) {

    if (pClient->isConnected()) {
        pClient->disconnect();

    }

    init();

    if (DEBUG) Serial.println("Start connecting...");

    pClient->connect(serverAddress);

    if (DEBUG) Serial.println("Client connected to server!");

    pRemoteService = pClient->getService(SERVICE_UUID);
    if (DEBUG) {
        Serial.print("Client found service: ");    
        Serial.println(pRemoteService->toString().c_str());
    } 

    if (pRemoteService == nullptr) {
        //Handle "Not able to connect to service"-error, probably not needed
        if (DEBUG) Serial.println("Not connected to service :(");
    } else {

        if (DEBUG) Serial.println("Connected! :)");
    }

    doConnect = false;
    return pRemoteService != nullptr;

}


bool E_BLE_C::addCharacteristic(const char *uuid) {
    if (DEBUG) Serial.println("Try to connect to characteritic");
    BLERemoteCharacteristic * remoteChar = pRemoteService->getCharacteristic(uuid);

    if (remoteChar == nullptr) {
        if (DEBUG) Serial.print("Could not find characteristic. UUID: ");
        return false;
    }

    // const uint8_t indicationOn[] = {0x2, 0x0};
    // remoteChar->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)indicationOn, 2, true);

    characteristics[uuid] = remoteChar;

    if (DEBUG) {
        Serial.print("Found characteristic! UUID: ");
        Serial.println(uuid);
    }

    if (remoteChar->canNotify()) {
        remoteChar->registerForNotify(notifyCallback);

    } 



    return remoteChar != nullptr;
}

bool E_BLE_C::validConnection = false;

void E_BLE_C::notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                                uint8_t *pData, size_t length, bool isNotify) {

    // char d[length];
    // for (int j = 0; j < length; ++j) {
    //     d[j] = pData[j];
    // }


    //    Serial.write(pData, length);
    Serial.println("KAKE!");
    String s = String((char*) pData);
    Serial.println(s);

    
    if (DEBUG) {
        Serial.print("Data: ");
        Serial.print(s.substring(0, length));
        Serial.println();
    }

}

void tryConnect(E_BLE_C *client, BLEAddress *serverAddress) {

    if (client->connect(*serverAddress)) {

        client->addCharacteristic(POWER_CHAR_UUID);
        // client->addCharacteristic(CHARACTERISTIC_UUID);
        // client->addCharacteristic(CONNECTION_CHAR_UUID);
    }
}


void E_BLE_C::init() {

    BLEClient *client = BLEDevice::createClient();

    if (DEBUG) Serial.println("Client created...");

    pClient = client;

}


bool E_BLE_C::begin() {

    BLEDevice::init(CLIENT_NAME);
    E_BLE_C::init();

    BLEScan *pBLEScan = BLEDevice::getScan();


    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedCallback(this));
    pBLEScan->setActiveScan(true);
    BLEScanResults result = pBLEScan->start(SCAN_DURATION);
    //TODO: something with result

    return true;
}


bool E_BLE_C::handle() {
    if (doConnect) {
        tryConnect(this, serverAddress);
    }

    validConnection = false;

    // delay(2000);

    // if (!validConnection) {

    //     if (DEBUG) Serial.println("\n\nSHUT DOWN!!!\n");
    //     doConnect = true;

    //     delay(10);
    // }


    // delay(10);
    return true;
}



// const uint8_t notificationOff[] = {0x0, 0x0};
// const uint8_t notificationOn[] = {0x1, 0x0};
// const uint8_t indicationOn[] = {0x2, 0x0};

// remoteChar->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)indicationOn, 2, true);
// remoteChar->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
