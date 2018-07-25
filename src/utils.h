
#ifndef UTILS_C
#define UTILS_C


#include <Arduino.h>
#include <Adafruit_GPS.h>

using namespace std;

union Coordinate {
    int c[2];
    char b[sizeof(int) * 2];
};


// Coordinate encodeGps(Adafruit_GPS g) {
//     float lat = g.latitude;
//     g.latitudeDegrees;


//     if (g.lat == 'S') lat *= -1;

//     if (g.lon == 'W') 




// }


String readLine() {
    
	String packet = "";
	for (char m = 0; m != 0x0D && m != 0x0A; m = Serial.read()) {
		if (m != 0x00 && m != 0xFF) packet += m;

		// if (m > 31 && m < 127) {
	    //     packet += m;

		// 	// packet = String(packet + m);
		// }
	}
	return packet;
}


Coordinate encodeLatLng(double latitude, double longitude, unsigned int precision = 7) {
    unsigned int prec = pow(10, precision);

    int lat = (int) (latitude * prec);
    int lng = (int) (longitude * prec);

    return Coordinate{lat, lng};
}

void decodeLatLng(Coordinate coordinate, unsigned int precision = 7) {
    long prec = pow(10, precision);

    char k[] = { 0xfd, 0x87, 0xb8, 0x23, 0xab, 0x3c, 0x63, 0x06 };

    String s = String(coordinate.c[0]);
    String g = s.substring(0, s.length() - precision) + '.' + s.substring(s.length() - precision);
    String s2 = String(coordinate.c[1]);
    String g2 = s2.substring(0, s2.length() - precision) + '.' + s2.substring(s2.length() - precision);

    Serial.print("Coordinates: ");
    Serial.print(g);
    Serial.print(" ");
    Serial.println(g2);





//    std::cout << lat << std::endl;
//    std::cout << lng << std::endl;




}





#endif /* UTILS_C */
