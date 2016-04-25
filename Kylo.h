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

#ifndef Kylo_h
#define Kylo_h

#include <Arduino.h>
#include "EEPROM.h"
#include "QTRSensors.h"

//DIRECCIONES DE LA EEPROM
#define LINEV_ADDRESS                                 0
#define KPLINE_ADDRESS                              sizeof(int)
#define KILINE_ADDRESS                                sizeof(int)+sizeof(double)*1
#define KDLINE_ADDRESS                              sizeof(int)+sizeof(double)*2
#define KPMAZE_ADDRESS                            sizeof(int)+sizeof(double)*3
#define KIMAZE_ADDRESS                              sizeof(int)+sizeof(double)*4
#define KDMAZE_ADDRESS                            sizeof(int)+sizeof(double)*5

#define MAZEV_ADDRESS                               sizeof(int)+sizeof(double)*6
#define TIME_ENTER_JUNCT_ADDRESS    sizeof(int)*2+sizeof(double)*6
#define TIME_ESC_JUNCT_ADDRESS         sizeof(int)*3+sizeof(double)*6
#define SPEED_TURN_ADDRESS                  sizeof(int)*4+sizeof(double)*6
#define ANGLE_TURN_ADDRESS                 sizeof(int)*5+sizeof(double)*6
#define SPEED_INVERT_ADDRESS              sizeof(int)*6+sizeof(double)*6
#define ANGLE_INVERT_ADDRESS             sizeof(int)*7+sizeof(double)*6

#define CALIBRATION_MIN             sizeof(int)*8+sizeof(double)*6
#define CALIBRATION_MAX 	sizeof(int)*8+sizeof(double)*6+sizeof(unsigned int)*8

//COMANDOS
#define LINEV_STRING              "LINEV"
#define KPLINE_STRING             "LINEKP"
#define KILINE_STRING             "LINEKI"
#define KDLINE_STRING             "LINEKD"
#define KPMAZE_STRING             "MAZEKP"
#define KIMAZE_STRING             "MAZEKI"
#define KDMAZE_STRING             "MAZEKD"
#define MAZEV_STRING              "MAZEV"
#define TIME_ENTER_JUNCT_STRING   "ENTERJUNCT"
#define TIME_ESC_JUNCT_STRING     "ESCJUNCT"
#define SPEED_TURN_STRING         "TURNSPEED"
#define ANGLE_TURN_STRING         "TURNANGLE"
#define SPEED_INVERT_STRING       "SPEEDINVERT"
#define ANGLE_INVERT_STRING       "ANGLEINVERT"

//MAZE
#define TIME_DECCEL       100
#define TIME_ACCEL        100
#define REFRESH_POS_RATE  10
#define	OVERLINE_SUM	4500

/*Clase Principal*/
class Kylo {
public:
	/* Constructor de Kylo*/
	Kylo(unsigned char* motorPins, unsigned char buttonPin);

	/* Controlador PID*/
	int getPID(double error);

	/* PID Debug */
	void getPIDvalues(int* Proportional, int* Integral, int* Derivative);

	/* Botón lateral*/
	bool getButton();

	/* Control de motores, aceleración y rotación */
	void motorSpeed(int speed, int rotation);

	void motorLinear(int initial_velocity, int final_velocity, float times,
			int rotation);

	void turnAxis(int speed_turn, unsigned int angle_turn, bool straightAfter =
			true);

	void LEDon();
	void LEDoff();
	double _KP;
	double _KI;
	double _KD;
	unsigned char _instanceV;

	/* Variables teoricamente protected, aqui para hacer debug*/
	/* ----------------------------------------------------------------------------------------*/

protected:
	/*Variables de Pines */
	unsigned char* _motorPins;
	unsigned char _buttonPin;

	/*Variables del PID */
	double _Error;
	double _lastError;
	double _derivative;
	long double _integral;
	double _PIDTime;

	/*Compensador de Motores*/
	int drift(int driftvalue);
};

/*Clase Line HEREDA de Kylo*/
class Line: public Kylo {

public:
	/* Constructor de Line, llama al constructor de Kylo*/
	Line(unsigned char* motorPins, unsigned char buttonPin);

	/* Actualizar KP, KI, KD */
	void refreshLinePID(double KPLine, double KILine, double KDLine);

	/* Control de EEPROM */
	bool setEEPROM(String command, double value);
	void setSerialEEPROM();

};

/*Clase Maze HEREDA de Kylo*/
class Maze: public Kylo {

public:
	/* Constructor de Maze, llama al constructor de Kylo*/
	Maze(unsigned char* motorPins, unsigned char buttonPin);

	/* Girar a la derecha o izquierda  90º, y darse la vuelta*/
	void invert();
	void turnRight();
	void turnLeft();

	void goBack(bool wasLeft);

	/* Control de EEPROM */
	bool setEEPROM(String command, double value);
	void setSerialEEPROM();

private:
	/*  Control de Tiempos del laberinto */
	int _time_enter_junct, _time_esc_junct;
	int _speed_turn, _angle_turn;
	int _speed_invert, _angle_invert;

};

/* Clase para controlar los Sharps */

class Sharps {
public:
	/* Constructor de sharp*/
	Sharps(unsigned char* sharpPins, int times, int leftThreshhold,
			int frontThreshold, int rightThreshhold);

	/* Funciones Booleanas */
	bool canLeft();bool canRight();bool canFront();

	void flagDoor(int times);

	/* Posicion DER-IZQ*/
	int getPos();

	int getRawFront();

	/* debug*/
	void getAll(int *left, int *front, int *right);

private:
	/* Variables para pines */
	unsigned char _leftPin;
	unsigned char _frontPin;
	unsigned char _rightPin;

	/* Variables de Control */
	int _times;
	int _leftThreshold;
	int _frontThreshold;
	int _rightThreshold;

	/* Haceer la media de los valores de uno de los sharps */
	int mediaValue(unsigned char pin);
};

/* Clase Control de la EEPROM */
class KyloEEPROM {

public:
	/* GETTERS */
	static int getLineV();
	static double getLineKp();
	static double getLineKi();
	static double getLineKd();
	static double getMazeKp();
	static double getMazeKi();
	static double getMazeKd();
	static int getMazeV();
	static int getTimeEnterJunct();
	static int getTimeEscJunct();
	static int getSpeedTurn();
	static int getAngleTurn();
	static int getSpeedInvert();
	static int getAngleInvert();
	static void getCalibratedMinimum(unsigned int values[]);
	static void getCalibratedMaximum(unsigned int values[]);
	static int getThresholdFront();
	static int getThresholdSide();

	/* SETTERS */
	static void setLineV(int value);
	static void setLineKp(double value);
	static void setLineKi(double value);
	static void setLineKd(double value);
	static void setMazeKp(double value);
	static void setMazeKi(double value);
	static void setMazeKd(double value);
	static void setMazeV(int value);
	static void setTimeEnterJunct(int value);
	static void setTimeEscJunct(int value);
	static void setSpeedTurn(int value);
	static void setAngleTurn(int value);
	static void setSpeedInvert(int value);
	static void setAngleInvert(int value);
	static void setCalibratedMinimum(unsigned int values[]);
	static void setCalibratedMaximum(unsigned int values[]);
	static void setThresholdFront(int value);
	static void setThresholdSide(int value);

	/* Limpieza de la EEPROM */
	static void clearEEPROM();
};

#endif
