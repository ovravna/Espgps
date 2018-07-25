#include "IridiumController.h"

void IridiumController::test() {
	Serial.println("TEST TEST TEST!");
	Serial.println("TEST TEST TEST!");
}

void IridiumController::begin() {
    Iridium.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
	
	pinMode(RI_PIN, INPUT);

	//Set auto retain; E1 = 9603 will always send back any valid command the user sends.
	if (debug) {
		pushCommand("ATE1", "OK");
	} else {
		pushCommand("ATE0", "OK");
	}

	//Clear MO buffer
	pushCommand("AT&D0", "OK");


	
	//Attatches to network
	pushCommand("AT+SBDREG?", "OK", [](IridiumController *crl){ 
		
		if (crl->status.REG == 2) {
			Serial.println("Attached to network");
			return; //OK
		}
		Serial.println("Failed to attatch...");
		//TODO: Attach manualty, may not be possible within the next 3 minutes, ref documentation.
		// pushCommand("AT+SBDREG", "OK");

	});

	// Sets flow controll to None (should be default)
	pushCommand("AT&K0", "OK"); 

#if true
	//Indicator Event Reporting, continually returns signal strength. Will probably use more power.
	pushCommand("AT+CIER=1,1", "OK");
#elif
	pushCommand("AT+CIER=0,0", "OK");
#endif
}

void IridiumController::handle() {

	// if (digitalRead(RI_PIN) == HIGH) {
	// 	Serial.println("RINGING!");
	// 	// pushCommand("AT+SBDIXA", "OK");
	// }


	while (Iridium.available() && !isActive()) {
		handleInterupt();
	}


	
    while (Iridium.available() && isActive()) {
		int n = handleResponse();
		if (n == 2) // ERROR
		{
			//TODO error handeling. It may be an idea to clear the sendQueue, however that may/will cause later commands/sessions to be deleted (NOT GOOD). Maybe its possible to design a session-state and only clear this session? 
			// However, I'm not actually sure it is needed at all, it seems like the 9603 is ignoring commands that are not valid.

			//Emptying queue
			// while (!sendQueue.isEmpty()) {
			// 	sendQueue.pop();
			// }
		}
    }


	while (!isActive() && !sendQueue.isEmpty()) {
		handleSend();

	}

}


bool IridiumController::isActive() {
	return commandActive;
}

bool IridiumController::setCommandState(bool active, String command, String endResponse) {
	commandActive = active;

	//Serial.println("Active: " + String(commandActive));

	if (active) {
		activeCommand = command;
		activeEndResponse = endResponse;
	} else {
		activeCommand = "";
		activeEndResponse = "";
	}
}

int IridiumController::handleR() {
	// 0 = Success, 1 = Not finished, 2 = Error

	String packet = readPacket();
	if (packet.length() == 0) return 1; 

	// Handle RING alert
	if (packet.startsWith("SBDRING")) {
		//The FA should append an ‘A’ to the command, i.e. +SBDIXA, when the SBD session is in response to an SBD ring alert. (ref. docs)
		pushCommand("AT+SBDIXA", "OK", [](IridiumController *crl){ 
			if (crl->status.MT_status == 1) {
				crl->pushCommand("AT+SBDRT", "OK");
			}
		});  

	}

	// Handle Event +XXXX:
	if (packet.startsWith("+")) {
		int st[6];
		int n = eventParser(packet, st);

		if (packet.startsWith("+SBDIX")) {
			status.update(st);
		} 
		else if (packet.startsWith("+CSQ")) {
			status.SignalQuality = st[0];
		} 
		else if (packet.startsWith("+SBDREG")) {
			status.REG = st[0];
		}
		else if (packet.startsWith("+CIEV")) {
			if (st[0] == 0) {
				status.SignalQuality = st[1];
			}
		} 
	}

	//Callback on pushCommand
	if (callbck.command == activeCommand) {
		if (isEndResponse(packet)) {
			callbck.responseCallback(this);
			callbck.clear();
		}
	}
	
	//Callback on received data
	if (activeCommand.startsWith("AT+SBDRT")) {
		if (!packet.startsWith("AT+SBDRT") && !packet.startsWith("+SBDRT:") && !packet.startsWith("OK")) {
			receivedSBDCallback(packet);
		}
	}

	//Generall response callback
	responseCallback(activeCommand, packet);
	
	//Checks for and of activity
	if (isEndResponse(packet)) {
		setCommandState(false);
		if (packet.startsWith("ERROR")) return 2;
		return 0;
	}



}

int IridiumController::handleInterupt() {
	String packet = readPacket();
	if (packet.length() == 0) return 2;

	if (packet.startsWith("+CIEV")) {
		int st[6];
		int l = eventParser(packet, st);
		if (st[0] == 0) {
			status.SignalQuality = st[1];
		}
		return 0;
	} 
	else if (packet.startsWith("SBDRING")) {
		//The FA should append an ‘A’ to the command, i.e. +SBDIXA, when the SBD session is in response to an SBD ring alert. (ref. docs)
		pushCommand("AT+SBDIXA", "OK", [](IridiumController *crl){ 
			if (crl->status.MT_status == 1) {
				crl->pushCommand("AT+SBDRT", "OK");
			}
		});  

	}

	Serial.println("Interupt: " + packet);
	return 0;
}

int IridiumController::handleResponse() {
	// 0 = Success, 1 = Not finished, 2 = Error

	String packet = readPacket();
	if (packet.length() == 0) return 1; 

	//Checks for status commands / events
	if (packet.startsWith("+SBDIX")) {
		int st[6];
		int n = eventParser(packet, st);
		status.update(st);
	} 
	else if (packet.startsWith("+CSQ")) {
		int st[1];
		int n = eventParser(packet, st);
		status.SignalQuality = st[0];
	} 
	else if (packet.startsWith("+SBDREG")) {
		int st[2];
		int n = eventParser(packet, st);
		status.REG = st[0];
	}
	
	//Callback on pushCommand
	if (callbck.command == activeCommand) {
		if (isEndResponse(packet)) {
			callbck.responseCallback(this);
			callbck.clear();
		}
	}
	
	//Callback on received data
	if (activeCommand.startsWith("AT+SBDRT")) {
		if (!packet.startsWith("AT+SBDRT") && !packet.startsWith("+SBDRT:") && !packet.startsWith("OK")) {
			receivedSBDCallback(packet);
		}
	}

	//Generall response callback
	responseCallback(activeCommand, packet);
	
	//Checks for and of activity
	if (isEndResponse(packet)) {
		setCommandState(false);
		if (packet.startsWith("ERROR")) return 2;
		return 0;
	}

	return 1;

}


int IridiumController::handleSend() {
	if (sendQueue.count() >= 2) {
		command(sendQueue.pop(), sendQueue.pop());
	}
 
	if (sendQueue.count() == 0) {
		//pass
	}
	return 0;

}

bool IridiumController::isEndResponse(String response) {
	if (response.startsWith("ERROR")) {
		return true;
	}

	if (activeEndResponse.equals(NUM) && response[0] >= '0' && response[0] <= '9') {
		return true;
	}
	
	if (response.startsWith(activeEndResponse)) {
		return true;
	}

	return false;
}




String IridiumController::checksum(String msg) {
	
	//NOTE: not sure if this is correct

  	uint16_t sum = 0;

	for (int i = 0; msg[i]; i++) {
		sum += (uint16_t)msg[i];
	}

	String s = "";
	s += (char)(sum >> 8);
	s += (char)(sum & 0xFF);
	
	return s;
}

bool IridiumController::pushCommand(String message, String response) {
	sendQueue.push(message);
	sendQueue.push(response);
}

bool IridiumController::pushCommand(String message, String response, void (*respCallback)(IridiumController *crl)) {
	pushCommand(message, response);
	callbck.init(message, respCallback);

}


void IridiumController::sendSBDtext(String message) {
	String len = String(message.length());
	String cs = checksum(message);

	pushCommand("AT+SBDWB=" + len, "READY");
	pushCommand(message + cs, "OK");

	// pushCommand("AT+SBDWT="+message, "OK"); 

	pushCommand("AT+SBDIX", "OK", [](IridiumController *crl) {
		if (crl->status.MT_queued == 0) {
			crl->pushCommand("AT+SBDD2", "OK"); //Clear buffer
		}
	});
	

}

String IridiumController::readPacket() {
	String packet = "";
	for (char m = 0; m != 0x0D && m != 0x0A; m = Iridium.read()) {
		if (m != 0x00 && m != 0xFF) packet += m;
	}
	return packet;
}

void IridiumController::end() {
        Iridium.write('\r');
	    Iridium.write('\n');
}

void IridiumController::setResponseCallcack(void (*respCallback)(String command, String response)){
	responseCallback = respCallback;
}

void IridiumController::setSBDCallcack(void (*sbdCallback)(String message)) {
	receivedSBDCallback = sbdCallback;
}

void IridiumController::write(String message) {
	Iridium.write(&message[0]);
	Serial.println("Sending: "+message);

	end();
}


bool IridiumController::command(String message, String response) {
    if (!isActive()) {
		// Serial.println("Writing " + message);
		setCommandState(true, message, response);
		write(message);
		return true;
    }

	return false;
}


int IridiumController::eventParser(String msg, int out[]) {
    int index = 0;
    String s = "";
	int j = msg.indexOf(':');
	if (j == -1) return -1;
	
    for (; j < msg.length(); j++) {
    
      while (msg[j] >= '0' && msg[j] <= '9') {
        s += msg[j++];
      }
      if (s.length() != 0) {
        out[index++] = s.toInt();
        s = "";
      }
    }

	return index + 1;

}