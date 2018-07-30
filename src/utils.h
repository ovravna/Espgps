
#ifndef UTILS_C
#define UTILS_C

#include <Arduino.h>





String readLine() {
    
	String packet = "";
	for (char m = 0; m != 0x0D && m != 0x0A; m = Serial.read()) {
		if (m != 0x00 && m != 0xFF) packet += m;
	}
	return packet;
}



#endif /* UTILS_C */
