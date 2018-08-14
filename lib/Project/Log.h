#ifndef LOG_H
#define LOG_H

#include <Arduino.h>



class Log {
    #define logger Serial

public:

    static const bool debug = true;


    static void println(String message) {
        if (debug) {
            logger.println(message);
        }
    } 

    static void print(String message) {
        if (debug) {
            logger.print(message);
        }
    } 




};


#endif