#include "Kylo.h"

Line::Line(unsigned char* motorPins, unsigned char buttonPin) :
		Kylo(motorPins, buttonPin) {
	_integral = 0;
	_instanceV = KyloEEPROM::getLineV();
	_KP = KyloEEPROM::getLineKp();
	_KI = KyloEEPROM::getLineKi();
	_KD = KyloEEPROM::getLineKd();

}

void Line::refreshLinePID(double KP, double KI, double KD) {
	_KP = KP;
	_KI = KI;
	_KD = KD;
	_integral = 0;
	KyloEEPROM::setLineKp(_KP);
	KyloEEPROM::setLineKi(_KI);
	KyloEEPROM::setLineKd(_KD);
}

bool Line::setEEPROM(String command, double value) {
	if (command == LINEV_STRING) {
		_instanceV = value;
		KyloEEPROM::setLineV(value);
		return true;
	} else if (command == KPLINE_STRING) {
		_KP = value;
		KyloEEPROM::setMazeKp(value);
		return true;
	} else if (command == KILINE_STRING) {
		_KI = value;
		KyloEEPROM::setMazeKi(value);
		return true;
	} else if (command == KDLINE_STRING) {
		_KD = value;
		KyloEEPROM::setMazeKd(value);
		return true;
	}
	return false;
}

void Line::setSerialEEPROM() {
	String command = Serial.readStringUntil(' ');
	double value = Serial.parseFloat();
	Serial.print("************************\n");
	Serial.print(command);
	Serial.print("          ");
	if (setEEPROM(command, value)) {
		Serial.println(value);
		Serial.print("************************\n");
	} else {
		Serial.println("Comando desconocido");
		Serial.print("************************\n");
	}
}
