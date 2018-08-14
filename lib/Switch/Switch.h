#ifndef Switch_H
#define Switch_H

#include <Arduino.h>
#define TEST_LED 14



class Switch {
    bool state = false;
    HardwareSerial serial; 
    int                 RX_PIN;
    int                 TX_PIN;

public:

    Switch(HardwareSerial hwSerial, int rxPin, int txPin): serial(hwSerial), RX_PIN(rxPin), TX_PIN(txPin) {
        pinMode(TEST_LED, OUTPUT);
        digitalWrite(TEST_LED, HIGH);
    }

    bool getState();  //returns state
    bool turnOff(); //returns state
    bool turnOn(); //returns state
    bool toggle(); //returns state


};


#endif /* Switch_H */