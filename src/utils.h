
#ifndef UTILS_C
#define UTILS_C

#include <Arduino.h>



unsigned long t2;
unsigned long t;
unsigned long pdiff;

void synchronize(unsigned long period = 1000) {

	pdiff = (millis() - t);
	if (pdiff < period) delay(period - pdiff);
	t = millis();
}


String readLine() {
    
	String packet = "";
	for (char m = 0; m != 0x0D && m != 0x0A; m = Serial.read()) {
		if (m != 0x00 && m != 0xFF) packet += m;
	}
	return packet;
}

#endif /* UTILS_C */
