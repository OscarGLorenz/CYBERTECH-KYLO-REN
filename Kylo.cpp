#include "Kylo.h"

Kylo::Kylo(unsigned char* motorPins, unsigned char buttonPin) {
	for (int i = 0; i < 4; i++) {
		pinMode(motorPins[i], OUTPUT);
	}
	pinMode(buttonPin, INPUT);
	_motorPins = motorPins;
	_buttonPin = buttonPin;
	_PIDTime = 0;
}

int Kylo::drift(int driftvalue) {
	if (driftvalue > 40) {
		long double calc = (long double) driftvalue;
		driftvalue = (int) ((-0.000007 * calc * calc + 0.0024 * calc + 0.7845)
				* calc);
	}
	return driftvalue;
}

void Kylo::LEDoff() {
	analogWrite(SDA, 0);
}

void Kylo::LEDon() {
	analogWrite(SDA, 255 * 2.6 / 5);
}

int Kylo::getPID(double error) {
	_Error = error;

	if (_Error > 3.45 && _lastError < -3.4) {
		_Error = -3.5;
	}
	_derivative = (_lastError - _Error);
	_integral += _Error;
	int _result = round(_Error * _KP + _integral * _KI + _derivative * _KD);
	_result = constrain(_result, -255, 255);
	_lastError = _Error;
	return _result;
}

void Kylo::getPIDvalues(int* Proportional, int* Integral, int* Derivative) {
	*Proportional = _KP * _Error;
	*Integral = _KI * _integral;
	*Derivative = _KD * _derivative;
}

bool Kylo::getButton() {
	return (analogRead(_buttonPin) > 1000);
}

void Kylo::turnAxis(int speed_turn, unsigned int angle_turn, bool straightAfter) {
	speed_turn = constrain(speed_turn, -255, 255);
	if (speed_turn > 0) {
		analogWrite(_motorPins[0], speed_turn);
		analogWrite(_motorPins[1], 0);
		analogWrite(_motorPins[2], 0);
		analogWrite(_motorPins[3], speed_turn);
	} else {
		analogWrite(_motorPins[0], 0);
		analogWrite(_motorPins[1], -speed_turn);
		analogWrite(_motorPins[2], -speed_turn);
		analogWrite(_motorPins[3], 0);
	}
	delay(angle_turn);
	if (straightAfter) {
	motorSpeed(255, 0);
	delay(1);
	}
}

void Kylo::motorSpeed(int speed, int rotation) {
	speed = constrain(speed, -255, 255);
	rotation = constrain(rotation, -255, 255);
	speed = map(speed, -255, 255, -_instanceV, _instanceV);
	rotation = map(rotation, -255, 255, -abs(speed), abs(speed));
	if (speed > 0) {
		if (rotation >= 0) {
			analogWrite(_motorPins[0], speed);
			analogWrite(_motorPins[1], 0);
			analogWrite(_motorPins[2], drift(speed));
			analogWrite(_motorPins[3], drift(rotation));
		} else {
			analogWrite(_motorPins[0], speed);
			analogWrite(_motorPins[1], -rotation);
			analogWrite(_motorPins[2], drift(speed));
			analogWrite(_motorPins[3], 0);
		}
	} else {
		if (rotation >= 0) {
			analogWrite(_motorPins[0], 0);
			analogWrite(_motorPins[1], -(speed));
			analogWrite(_motorPins[2], drift(rotation));
			analogWrite(_motorPins[3], -drift(speed));
		} else {
			analogWrite(_motorPins[0], -rotation);
			analogWrite(_motorPins[1], -speed);
			analogWrite(_motorPins[2], 0);
			analogWrite(_motorPins[3], -drift(speed));
		}
	}
}

void Kylo::motorLinear(int initialVelocity, int initialFinal,
		float timeAcceleration, int motorRotation) {
	double acceleration = (initialFinal - initialVelocity) / timeAcceleration;
	for (int i = 0; i < timeAcceleration; i++) {
		motorSpeed(motorRotation, initialVelocity + acceleration * i);
		delay(1);
	}
}
