#include "Switch.h"


bool Switch::getState() {
    return state;
}
 
bool Switch::toggle() {
    return state ? turnOff() : turnOff();
}

bool Switch::turnOff() {

    digitalWrite(TEST_LED, LOW);

    serial.write("smsc$0%%$0%%");

    state = false;

    return state;
}

bool Switch::turnOn() {
    digitalWrite(TEST_LED, HIGH);

    serial.write("smsc$1%%$1%%");
    state = true;

    return state;
}

