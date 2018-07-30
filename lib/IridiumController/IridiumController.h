#ifndef IDIDIUM_PARSER_H
#define IDIDIUM_PARSER_H

#include <Arduino.h>
#include <QueueList.h>
#include <IDevice.h>
#include <UUIDs.h>



#define NUM "\\d"

class IridiumController: public IDevice {

    const unsigned int  BAUD_RATE = 19200;

    bool                commandActive = false;
    String              activeEndResponse; 
    String              activeCommand; 

    bool                sbdActive = false;

    HardwareSerial      Iridium;

    int                 RX_PIN, TX_PIN, RI_PIN;

    void (*responseCallback)(String command, String response) = [](String s, String z){return;};
    void (*receivedSBDCallback)(String message) =  [](String s){return;};

    QueueList <String> sendQueue; 

    void end();

    int handleResponse();
    int handleSend();
    int handleInterupt();
    int handleR();


    bool isEndResponse(String response);

    String readPacket();

    String checksum(String msg);
    void write(String message);

    bool isActive();
    bool setCommandState(bool active, String command = "", String endResponse = "");
    bool command(String message, String response);

    /*
    `Coordinates` is used for simple lossless compression of a GPS coordinate for packaging in SBD write. 
    It turns two int32s into an 8 bytes. 
    */
    union Coordinate {
        int coordinates[2];
        char bytes[sizeof(int) * 2];
    };

    

    struct {
        String command;
        void (*responseCallback)(IridiumController *crl);
        void init(String cmd, void (*respCallback)(IridiumController *crl)) {
            command = cmd;
            responseCallback = respCallback;
        }

        void clear() {
            command = ""; 
            responseCallback = nullptr;
        }

    } callbck; 


public:


    explicit IridiumController(HardwareSerial hws, int rx, int tx, int ri): Iridium(hws), RX_PIN(rx), TX_PIN(tx), RI_PIN(ri) {        

    }

    bool begin() override;

    bool handle() override;

    void test();

    void setResponseCallcack(void (*respCallback)(String command, String response));
    void setSBDCallcack(void (*sbdCallback)(String message));

    void sendSBDtext(String message);

    bool pushCommand(String message, String response);
    bool pushCommand(String message, String response, void (*respCallback)(IridiumController *crl));
  

    struct {

        int MO_status; 
        /*
        MO session status provides an indication of the disposition of the mobile originated transaction. 
        The field can take on the following values:

        Gateway-reported values:
            0 MO message, if any, transferred successfully.
            1 MO message, if any, transferred successfully, but the MT message in the queue was too
            big to be transferred.
            2 MO message, if any, transferred successfully, but the requested Location Update was
            not accepted.
            3..4 Reserved, but indicate MO session success if used.
            5..8 Reserved, but indicate MO session failure if used.
            10 GSS reported that the call did not complete in the allowed time.
            11 MO message queue at the GSS is full.
            12 MO message has too many segments.
            13 GSS reported that the session did not complete.
            14 Invalid segment size.
            15 Access is denied.
        ISU-reported values:
            16 ISU has been locked and may not make SBD calls (see +CULK command).
            17 Gateway not responding (local session timeout).
            18 Connection lost (RF drop).
            19 Link failure (A protocol error caused termination of the call).
            20..31 Reserved, but indicate failure if used.
            32 No network service, unable to initiate call.
            33 Antenna fault, unable to initiate call.
            34 Radio is disabled, unable to initiate call (see *Rn command).
            35 ISU is busy, unable to initiate call.
            36 Try later, must wait 3 minutes since last registration.
            37 SBD service is temporarily disabled.
            38 Try later, traffic management period (see +SBDLOE command)
            39..63 Reserved, but indicate failure if used.
            64 Band violation (attempt to transmit outside permitted frequency band).
            65 PLL lock failure; hardware error during attempted transmit.
        */

        int MOMSN; //The Mobile Originated Message Sequence Number.
        
        int MT_status; 
        /* 
        The MT status provides an indication of the disposition of the mobile terminated transaction. 
        The field can take on the following values:
            0 No SBD message to receive from the GSS.
            1 SBD message successfully received from the GSS.
            2 An error occurred while attempting to perform a mailbox check or receive a message from the GSS.
        */


        int MTMSN; //The Mobile Terminated Message Sequence Number.
        
        int MT_length; //The MT length is the length in bytes of the mobile terminated SBD message received from the GSS. If no message was received, this field will be zero.
        
        
        int MT_queued; //MT queued is a count of mobile terminated SBD messages waiting at the GSS to be transferred to the ISU.

        int SignalQuality = -1; // The received signal strength indication <rssi> from the ISU.

        int REG; //Network Registration Status
        /*
        <status> indicates the resulting registration status of the ISU:
            0 Detached – ISU is detached as a result of a successful +SBDDET or +SBDI command.
            1 Not registered – ISU is attached but has not provided a good location since it was last detached.
            2 Registered – ISU is attached with a good location. Note that this may be the case even when the most recent attempt did not provide a good location.
            3 Registration denied – The GSS is denying service to the ISU.
        */

        void update(int s[6]) {
            int i = 0;
            MO_status = s[i++];
            MOMSN = s[i++];
            MT_status = s[i++];
            MTMSN = s[i++];
            MT_queued = s[i++];
        }

    } status;

    static int eventParser(String sbdi, int out[]);

      
    static char* bytify(int lat, int lng) {
        return Coordinate { lat, lng }.bytes;
    }

    static void unbytify(char byteLatLng[8], int * outLatLng) {
        Coordinate coor; 
        for (int i = 0; i < 8; i++) {
            coor.bytes[i] = byteLatLng[i];

        }
        outLatLng[0] = coor.coordinates[0];
        outLatLng[1] = coor.coordinates[1];
    }

};





#endif