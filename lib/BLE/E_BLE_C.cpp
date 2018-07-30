#include "E_BLE_C.h"
#include <UUIDs.h>




void AdvertisedCallback::onResult(BLEAdvertisedDevice advertisedDevice) {

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

    if (DEBUG) Serial.println("Start connecting...");

    pClient->connect(serverAddress);

    if (DEBUG) Serial.println("Client connected to server!");

    pRemoteService = pClient->getService(SERVICE_UUID);
    if (DEBUG) {
        Serial.print("Client found service: ");    
        Serial.println(pRemoteService->toString().c_str());
    } 

    if (pRemoteService == nullptr) {
        //TODO: something...
        if (DEBUG) Serial.println("Not connected to service :(");
    } else {

        if (DEBUG) Serial.println("Connected! :)");
    }

    doConnect = false;
    return pRemoteService != nullptr;

}


bool E_BLE_C::addCharacteristic(const char *uuid) {
    BLERemoteCharacteristic * remoteChar = pRemoteService->getCharacteristic(uuid);

    if (remoteChar == nullptr) {
        if (DEBUG) Serial.print("Could not find characteristic. UUID: ");
        return false;
    }
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

    char d[length];
    for (int j = 0; j < length; ++j) {
        d[j] = pData[j];
    }


    //    Serial.write(pData, length);

    Serial.write("Data: ");
    Serial.write(pData, length);
    Serial.println();
//     Serial.write(" | Length: ");
//     Serial.print(length);
//     Serial.write(" | ");

// //    sdata[sindex] = String(d);

//     if (String(d).startsWith("OK")) {
//         validConnection = true;
//         Serial.write("+");
//     } else {
//         Serial.write("-");
//     }
//     Serial.write('\n');

}

void tryConnect(E_BLE_C *client, BLEAddress *serverAddress) {

    if (client->connect(*serverAddress)) {

        client->addCharacteristic(CHARACTERISTIC_UUID);
        client->addCharacteristic(POWER_CHAR_UUID);
        client->addCharacteristic(CONNECTION_CHAR_UUID);
    }
}


void E_BLE_C::init() {

    BLEClient *client = BLEDevice::createClient();

    Serial.println("Client created...");

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

    if (!validConnection) {

        Serial.println("\n\nSHUT DOWN!!!\n");
        doConnect = true;

        delay(10);
    }
    delay(10);
    return true;
}

