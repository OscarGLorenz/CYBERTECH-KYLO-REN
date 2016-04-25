/*  
    Programa del Equipo KYLO REN
    Copyright (C) 2016  KyloRen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Kylo.h"

/* GETTERS */
int KyloEEPROM::getLineV() {
	int value;
	EEPROM.get(LINEV_ADDRESS, value);
	return value;
}
double KyloEEPROM::getLineKp() {
	double value;
	EEPROM.get(KPLINE_ADDRESS, value);
	return value;
}
double KyloEEPROM::getLineKi() {
	double value;
	EEPROM.get(KILINE_ADDRESS, value);
	return value;
}
double KyloEEPROM::getLineKd() {
	double value;
	EEPROM.get(KDLINE_ADDRESS, value);
	return value;
}
double KyloEEPROM::getMazeKp() {
	double value;
	EEPROM.get(KPMAZE_ADDRESS, value);
	return value;
}
double KyloEEPROM::getMazeKi() {
	double value;
	EEPROM.get(KIMAZE_ADDRESS, value);
	return value;
}
double KyloEEPROM::getMazeKd() {
	double value;
	EEPROM.get(KDMAZE_ADDRESS, value);
	return value;
}
int KyloEEPROM::getMazeV() {
	int value;
	EEPROM.get(MAZEV_ADDRESS, value);
	return value;
}
int KyloEEPROM::getTimeEnterJunct() {
	int value;
	EEPROM.get(TIME_ENTER_JUNCT_ADDRESS, value);
	return value;
}
int KyloEEPROM::getTimeEscJunct() {
	int value;
	EEPROM.get(TIME_ESC_JUNCT_ADDRESS, value);
	return value;
}
int KyloEEPROM::getSpeedTurn() {
	int value;
	EEPROM.get(SPEED_TURN_ADDRESS, value);
	return value;
}
int KyloEEPROM::getAngleTurn() {
	int value;
	EEPROM.get(ANGLE_TURN_ADDRESS, value);
	return value;
}
int KyloEEPROM::getSpeedInvert() {
	int value;
	EEPROM.get(SPEED_INVERT_ADDRESS, value);
	return value;
}
int KyloEEPROM::getAngleInvert() {
	int value;
	EEPROM.get(ANGLE_INVERT_ADDRESS, value);
	return value;
}

void KyloEEPROM::getCalibratedMinimum(unsigned int values[]) {
	for (int i = 0; i < 8; i++) {
		int value;
		EEPROM.get(CALIBRATION_MIN + i * sizeof(unsigned int), value);
		values[i] = value;
	}
}

void KyloEEPROM::getCalibratedMaximum(unsigned int values[]) {
	for (int i = 0; i < 8; i++) {
		int value;
		EEPROM.get(CALIBRATION_MAX + i * sizeof(unsigned int), value);
		values[i] = value;
	}
}



/* SETTERS */

void KyloEEPROM::setLineV(int value) {
	EEPROM.put(LINEV_ADDRESS, value);
}
void KyloEEPROM::setLineKp(double value) {
	EEPROM.put(KPLINE_ADDRESS, value);
}
void KyloEEPROM::setLineKi(double value) {
	EEPROM.put(KILINE_ADDRESS, value);
}
void KyloEEPROM::setLineKd(double value) {
	EEPROM.put(KDLINE_ADDRESS, value);
}
void KyloEEPROM::setMazeKp(double value) {
	EEPROM.put(KPMAZE_ADDRESS, value);
}
void KyloEEPROM::setMazeKi(double value) {
	EEPROM.put(KIMAZE_ADDRESS, value);
}
void KyloEEPROM::setMazeKd(double value) {
	EEPROM.put(KDMAZE_ADDRESS, value);
}
void KyloEEPROM::setMazeV(int value) {
	EEPROM.put(MAZEV_ADDRESS, value);
}
void KyloEEPROM::setTimeEnterJunct(int value) {
	EEPROM.put(TIME_ENTER_JUNCT_ADDRESS, value);
}
void KyloEEPROM::setTimeEscJunct(int value) {
	EEPROM.put(TIME_ESC_JUNCT_ADDRESS, value);
}
void KyloEEPROM::setSpeedTurn(int value) {
	EEPROM.put(SPEED_TURN_ADDRESS, value);
}
void KyloEEPROM::setAngleTurn(int value) {
	EEPROM.put(ANGLE_TURN_ADDRESS, value);
}
void KyloEEPROM::setSpeedInvert(int value) {
	EEPROM.put(SPEED_INVERT_ADDRESS, value);
}
void KyloEEPROM::setAngleInvert(int value) {
	EEPROM.put(ANGLE_INVERT_ADDRESS, value);
}

void KyloEEPROM::setCalibratedMinimum(unsigned int values[]) {
	for (int i = 0; i < 8; i++) {
		EEPROM.put(CALIBRATION_MIN + i * sizeof(unsigned int) , values[i]);
	}
}
void KyloEEPROM::setCalibratedMaximum(unsigned int values[]) {
	for (int i = 0; i < 8; i++) {
		EEPROM.put(CALIBRATION_MAX + i * sizeof(unsigned int) , values[i]);
	}
}

void KyloEEPROM::clearEEPROM() {
	for (unsigned int i = 0; i < EEPROM.length(); i++) {
		EEPROM.write(i, 1);
	}
}

