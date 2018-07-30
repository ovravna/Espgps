#ifndef Switch_H
#define Switch_H

#include <Arduino.h>


class Switch {
    bool state = false;

public:

    bool getState();  //returns state
    bool turnOff(); //returns state
    bool turnOn(); //returns state
    bool toggle(); //returns state


};


#endif /* Switch_H */