#ifndef Switch_H
#define Switch_H

#include <Arduino.h>
#define TEST_LED 14



class Switch {
    bool state = false;

public:

    Switch() {
        pinMode(TEST_LED, OUTPUT);
        digitalWrite(TEST_LED, HIGH);
    }

    bool getState();  //returns state
    bool turnOff(); //returns state
    bool turnOn(); //returns state
    bool toggle(); //returns state


};


#endif /* Switch_H */