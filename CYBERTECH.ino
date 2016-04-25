#include "Kylo.h"
#include "QTRSensors.h"

//PINES DEL ARDUINO
#define ARRAY_PINS   (unsigned char[]) {2, 3, 4, 7, 8, 9, 12, 13}
#define ARRAY_SENSOR_NUMBER  8
#define ARRAY_TIMEOUT    2500
#define ARRAY_CLOCKPIN   SCL
#define MOTOR_PINS   (unsigned char[]) {10, 11, 5 ,6}
#define SHARP_PINS   (unsigned char[]) {A0, A1, A2}
#define BUTTON_PIN   A3

// OTROS PARAMETROS
#define LEFT_THRESHOLD  300
#define FRONT_THRESHOLD  280
#define RIGHT_THRESHOLD  300
#define ITERATIONS_LINE	5

#define CURVE_SLOW  200
#define SLOW_SPEED  170
#define FAST_SPEED  255

//SHARPS LABERINTO
#define DIST_LEFT_MAZE    280
#define DIST_FRONT_MAZE   200
#define DIST_RIGHT_MAZE	280
#define ITERATIONS_MAZE 5

#define ROTACION_A_PELO	-10
#define LINEA_CERO 400

//PARAMETROS LABERINTO
#define DELAY_INIT        1000
#define DELAY_CONFIG      500
#define DELAY_READY       3000
#define TIME_BEGIN_MAZE   200

//DECLARACIÓN DE OBJETOS

QTRSensorsRC lineArray(ARRAY_PINS, ARRAY_SENSOR_NUMBER, ARRAY_TIMEOUT,
ARRAY_CLOCKPIN);

Line line(MOTOR_PINS, BUTTON_PIN);

Sharps lineSharps(SHARP_PINS, ITERATIONS_LINE, LEFT_THRESHOLD, FRONT_THRESHOLD,
RIGHT_THRESHOLD);

Maze maze(MOTOR_PINS, BUTTON_PIN);

Sharps mazeSharps(SHARP_PINS, ITERATIONS_MAZE, DIST_LEFT_MAZE, DIST_FRONT_MAZE,
DIST_RIGHT_MAZE);

unsigned int sensorValues[8];
double errorLine = 0;
double rotation = 0;

//*********************************************************************
//******************************* DATOS LINEA**************************
//*********************************************************************

void lineDebug() {
	int myP, myI, myD;
	int a, b, c;
	line.getPIDvalues(&myP, &myI, &myD);
	lineSharps.getAll(&a, &b, &c);
	Serial.print(errorLine);
	Serial.print("\t");
	Serial.print((rotation));
	Serial.print("\t");
	Serial.print(lineSharps.canFront());
	Serial.print("\t");
	Serial.print(b);
	Serial.print("\t");
	Serial.print(line.getButton());
	Serial.print("\t");
	Serial.print(myP);
	Serial.print("\t");
	Serial.print(myI);
	Serial.print("\t");
	Serial.print(myD);
	Serial.print("\t");
	Serial.print(line._KP);
	Serial.print("\t");
	Serial.print(line._KI);
	Serial.print("\t");
	Serial.print(line._KD);
	Serial.print("\n");
}

void Bluetooth() {
	String command = Serial.readStringUntil(' ');
	if (command == "PID") {
		double KP_LINE = Serial.parseFloat();
		delay(10);
		double KI_LINE = Serial.parseFloat();
		delay(10);
		double KD_LINE = Serial.parseFloat();
		delay(10);
		int lineV = Serial.parseInt();
		Serial.println("\nNuevos Parametros");
		Serial.print(KP_LINE, 3);
		Serial.print("\t");
		Serial.print(KI_LINE, 3);
		Serial.print("\t");
		Serial.print(KD_LINE, 3);
		Serial.print("\t");
		line.refreshLinePID(KP_LINE, KI_LINE, KD_LINE);
		KyloEEPROM::setLineV(lineV);
	} else if (command == "CAL") {
		delay(500);
		line.LEDon();

		for (int i = 0; i < 400; i++) {
			lineArray.calibrate();
		}

		for (int i = 0; i < 8; i++) {
			Serial.print(lineArray.calibratedMinimumOn[i]);
			Serial.print(' ');
		}
		Serial.println();
		for (int i = 0; i < 8; i++) {
			Serial.print(lineArray.calibratedMaximumOn[i]);
			Serial.print(' ');
		}

		KyloEEPROM::setCalibratedMinimum(lineArray.calibratedMinimumOn);
		KyloEEPROM::setCalibratedMaximum(lineArray.calibratedMaximumOn);

		line.LEDoff();
	} else {
		double value = Serial.parseFloat();
		line.setEEPROM(command, value);
	}
}

//**********************************************************************
//************************ DATOS LABERINTO *****************************
//**********************************************************************

int repeat_debug = 0;
void mazeDebug() {
	if (!(repeat_debug % 1000)) {
		int left, front, right;
		mazeSharps.getAll(&left, &front, &right);
		Serial.print("SENSORES:   IZQIERDA: ");
		Serial.print(left);
		Serial.print(" >> ");
		Serial.print(mazeSharps.canLeft());
		Serial.print(",   CENTRO: ");
		Serial.print(front);
		Serial.print(" >> ");
		Serial.print(mazeSharps.canFront());
		Serial.print(",   DERECHA: ");
		Serial.print(right);
		Serial.print(" >> ");
		Serial.print(mazeSharps.canRight());
		Serial.print(",   POSICION: ");
		Serial.println(mazeSharps.getPos());
	}
	repeat_debug++;
}

//*********************************************************************
//********************* ALGORITMO SIGUELINEAS **********************
//*********************************************************************

double getLine() {
	return (lineArray.readLine(sensorValues) - 3500.0) / 1000.0;
}

//*********************************************************************
//********************* ALGORITMO LABERINTO **********************
//*********************************************************************

void BluetoothMaze() {
	maze.setSerialEEPROM();
}
//*********************************************************************
//**************************** MAIN CODE ****************************
//*******************************************************************
#define DIM_OF_STUCK	10
int stuckArray[DIM_OF_STUCK] = { 0 };
int stuckArray2[DIM_OF_STUCK] = { 0 };

int stuckIndex = 0;
int stuckIndex2 = 0;

int nowStuck = 0;

bool wasLeft = false;

double standardDeviation(int myArray[]) {
	double average;
	int sum = 0;

	for (int i = 0; i < DIM_OF_STUCK; i++) {
		sum += myArray[i];
	}
	average = sum / ((double) DIM_OF_STUCK);

	double sumFactor = 0.0;

	for (int i = 0; i < DIM_OF_STUCK; i++) {
		sumFactor += pow((myArray[i] - average), 2);
	}

	double varianza = sumFactor / ((double) (DIM_OF_STUCK - 1));
	return (sqrt(varianza));
}

void stuck() {
	if (nowStuck % 5 == 0) {
		if (stuckIndex == 10) {
			stuckIndex = 0;
		}
		if (stuckIndex2 == 10) {
			stuckIndex2 = 0;
		}
		stuckArray[stuckIndex] = mazeSharps.getRawFront();
		stuckArray2[stuckIndex2] = mazeSharps.getPos();

		if (standardDeviation(stuckArray) < 2.5
				|| standardDeviation(stuckArray2) < 2.5) {
			maze.goBack(wasLeft);
		}

		stuckIndex++;
	}
	nowStuck++;
}

void retrieveCalibration() {
	lineArray.calibratedMaximumOn = (unsigned int*) malloc(
			sizeof(unsigned int) * 8);
	lineArray.calibratedMinimumOn = (unsigned int*) malloc(
			sizeof(unsigned int) * 8);

	KyloEEPROM::getCalibratedMaximum(lineArray.calibratedMaximumOn);
	KyloEEPROM::getCalibratedMinimum(lineArray.calibratedMinimumOn);
}

void mainLine() {
	errorLine = getLine();
	rotation = -line.getPID(errorLine);
	line.motorSpeed(FAST_SPEED, rotation);
	if (Serial.available() > 0)
		Bluetooth();
}

bool mainCondition() {
	for (int i = 0; i < 5; i++) {
		if (lineSharps.canRight()) {
			return true;
		}
	}

	for (int i = 0; i < 30; i++) {
		if (lineSharps.canRight() || lineSharps.canLeft()) {
			return true;
		}
	}
	return false;
}

bool canRightForSure() {
	for (int i = 0; i < 5; i++) {
		if (!lineSharps.canRight()) {
			return false;
		}
	}
	return true;
}

bool canFrontForSure() {
	for (int i = 0; i < 5; i++) {
		if (!lineSharps.canFront()) {
			return false;
		}
	}
	return true;
}

void enterMaze() {
	maze.motorSpeed(255, ROTACION_A_PELO);
	while (canFrontForSure()) {
		delay(10);
	}
}

void mainMaze() {
	if (Serial.available() > 0)
		BluetoothMaze();

	stuck();
	mazeDebug();
	if (canRightForSure()) {
		analogWrite(SDA, 2.6 / 5 * 255);
		delay(30);
		analogWrite(SDA, 0);
		maze.turnRight(); //<----------- AJUSTAR
		wasLeft = false;
	} else if (canFrontForSure()) {

		for (int i = 0; i < 30; i++) {
			mazeDebug();
			maze.motorSpeed(FAST_SPEED,
					-maze.getPID(mazeSharps.getPos() - LINEA_CERO));

			if (Serial.available() > 0)
				BluetoothMaze();

			delay(10);

		}
	} else {
		maze.turnLeft();
		wasLeft = true;
	}
}

void setup() {
	Serial.begin(9600);
	Serial.setTimeout(10);
	pinMode(SDA, OUTPUT);

	delay(25);

	retrieveCalibration();

	delay(10);

	lineSharps.flagDoor(6);

	line.motorSpeed(FAST_SPEED, 0);

	delay(10);

}

void loop() {
	mainLine();
	do {
	} while (mainCondition());

	enterMaze();

	for (;;) {
		mainMaze();
	}
}
