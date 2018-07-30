#include "Switch.h"


bool Switch::getState() {
    return state;
}
 
bool Switch::toggle() {
    return state ? turnOff() : turnOff();
}

bool Switch::turnOff() {

    digitalWrite(TEST_LED, LOW);
    state = digitalRead(TEST_LED);

    return state;
}

bool Switch::turnOn() {
    digitalWrite(TEST_LED, HIGH);
    state = digitalRead(TEST_LED);

    return state;
}

