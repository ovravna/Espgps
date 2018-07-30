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

    Serial.write("smsc$0%%$0%%");

    return state;
}

bool Switch::turnOn() {
    digitalWrite(TEST_LED, HIGH);
    state = digitalRead(TEST_LED);

    Serial.write("smsc$1%%$1%%");
    return state;
}

