#include "Kylo.h"

Maze::Maze(unsigned char* motorPins, unsigned char buttonPin) :
		Kylo(motorPins, buttonPin) {
	_KP = KyloEEPROM::getMazeKp();
	_KI = KyloEEPROM::getMazeKi();
	_KD = KyloEEPROM::getMazeKd();
	_instanceV = KyloEEPROM::getMazeV();
	_time_enter_junct = KyloEEPROM::getTimeEnterJunct();
	_time_esc_junct = KyloEEPROM::getTimeEscJunct();
	_speed_turn = KyloEEPROM::getSpeedTurn();
	_angle_turn = KyloEEPROM::getAngleTurn();
	_speed_invert = KyloEEPROM::getSpeedInvert();
	_angle_invert = KyloEEPROM::getAngleInvert();
}

void Maze::invert() {
	Serial.println("KYLO IS TURNING BACK");
	analogWrite(_motorPins[0], _speed_invert * 0.75);
	analogWrite(_motorPins[1], 0);
	analogWrite(_motorPins[2], 0);
	analogWrite(_motorPins[3], _speed_invert);
	delay(_angle_invert);
}

void Maze::goBack(bool wasLeft) {
	analogWrite(_motorPins[0], 0);
	analogWrite(_motorPins[1], _instanceV);
	analogWrite(_motorPins[2], 0);
	analogWrite(_motorPins[3], _instanceV);
	delay(500);
	if (wasLeft) {
		turnLeft();
	} else {
		turnRight();
	}
}

void Maze::turnRight() {
	Serial.println("KYLO IS TURNING RIGHT");
	delay(_time_enter_junct);
	analogWrite(_motorPins[0], _speed_turn * 0.75);
	analogWrite(_motorPins[1], 0);
	analogWrite(_motorPins[2], 0);
	analogWrite(_motorPins[3], _speed_turn);
	delay(_angle_turn);
	motorSpeed(255, 0);
	delay(_time_esc_junct);

}

void Maze::turnLeft() {
	Serial.println("KYLO IS TURNING LEFT");
	delay(_time_enter_junct);
	analogWrite(_motorPins[0], 0);
	analogWrite(_motorPins[1], _speed_turn);
	analogWrite(_motorPins[2], _speed_turn * 0.75);
	analogWrite(_motorPins[3], 0);
	delay(_angle_turn);
	motorSpeed(255, 0);
	delay(_time_esc_junct);

}

//*********************************************************************
//******************************* DATOS *******************************
//*********************************************************************

bool Maze::setEEPROM(String command, double value) {
	if (command.equalsIgnoreCase("EEPROM")) {
		Serial.println("\n****************************************");
		Serial.print("KP = ");
		Serial.println(KyloEEPROM::getMazeKp());
		Serial.print("KI = ");
		Serial.println(KyloEEPROM::getMazeKi());
		Serial.print("KD = ");
		Serial.println(KyloEEPROM::getMazeKd());
		Serial.print("MAZEV = ");
		Serial.println(KyloEEPROM::getMazeV());
		Serial.print("ENT JUNCT = ");
		Serial.println(KyloEEPROM::getTimeEnterJunct());
		Serial.print("ESC JUNCT = ");
		Serial.println(KyloEEPROM::getTimeEscJunct());
		Serial.print("SPEED TURN = ");
		Serial.println(KyloEEPROM::getSpeedTurn());
		Serial.print("ANGLE TURN = ");
		Serial.println(KyloEEPROM::getAngleTurn());
		Serial.print("SPEED INVERT = ");
		Serial.println(KyloEEPROM::getSpeedInvert());
		Serial.print("ANGLE INVERT = ");
		Serial.println(KyloEEPROM::getAngleInvert());
		Serial.println("****************************************");
	} else if (command.equalsIgnoreCase(KPMAZE_STRING)) {
		_KP = value;
		KyloEEPROM::setMazeKp(value);
		return true;
	} else if (command.equalsIgnoreCase(KIMAZE_STRING)) {
		_KI = value;
		KyloEEPROM::setMazeKi(value);
		return true;
	} else if (command.equalsIgnoreCase(KDMAZE_STRING)) {
		_KD = value;
		KyloEEPROM::setMazeKd(value);
		return true;
	} else if (command.equalsIgnoreCase(MAZEV_STRING)) {
		_instanceV = value;
		KyloEEPROM::setMazeV(value);
		return true;
	} else if (command.equalsIgnoreCase(TIME_ENTER_JUNCT_STRING)) {
		_time_enter_junct = value;
		KyloEEPROM::setTimeEnterJunct(value);
		return true;
	} else if (command.equalsIgnoreCase(TIME_ESC_JUNCT_STRING)) {
		_time_esc_junct = value;
		KyloEEPROM::setTimeEscJunct(value);
		return true;
	} else if (command.equalsIgnoreCase(SPEED_TURN_STRING)) {
		_speed_turn = value;
		KyloEEPROM::setSpeedTurn(value);
		return true;
	} else if (command.equalsIgnoreCase(ANGLE_TURN_STRING)) {
		_angle_turn = value;
		KyloEEPROM::setAngleTurn(value);
		return true;
	} else if (command.equalsIgnoreCase(SPEED_INVERT_STRING)) {
		_speed_invert = value;
		KyloEEPROM::setSpeedInvert(value);
		return true;
	} else if (command.equalsIgnoreCase(ANGLE_INVERT_STRING)) {
		_angle_invert = value;
		KyloEEPROM::setAngleInvert(value);
		return true;
	}
	return false;
}

void Maze::setSerialEEPROM() {
	String command = Serial.readStringUntil(' ');
	double value = Serial.parseFloat();
	Serial.print("************************\n ");
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
