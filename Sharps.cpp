#include "Kylo.h"

Sharps::Sharps(unsigned char* sharpPins, int times, int leftThreshhold,
		int frontThreshold, int rightThreshhold) {

	_leftPin = sharpPins[0];
	_frontPin = sharpPins[1];
	_rightPin = sharpPins[2];

	_times = times;

	_leftThreshold = leftThreshhold;
	_frontThreshold = frontThreshold;
	_rightThreshold = rightThreshhold;
}

int Sharps::mediaValue(unsigned char pin) {
	int sum = 0;
	for (int i = 0; i < _times; i++) {
		sum += analogRead(pin);
		delay(1);
	}
	return (sum / _times);
}

bool Sharps::canLeft() {
	return (mediaValue(_leftPin) < _leftThreshold);
}
bool Sharps::canFront() {
	return (mediaValue(_frontPin) < _frontThreshold);
}
bool Sharps::canRight() {
	return (mediaValue(_rightPin) < _rightThreshold);
}

int Sharps::getPos() {
	return (mediaValue(_rightPin));
}

int Sharps::getRawFront() {
	return (mediaValue(_frontPin));
}

void Sharps::flagDoor(int times) {
	canFront();
	delay(10);
	for (int i = 0; i < times; i++) {
		while (!canFront()) {
			delay(10);
		}
	}
}

void Sharps::getAll(int *left, int *front, int *right) {
	*left = mediaValue(_leftPin);
	*front = mediaValue(_frontPin);
	*right = mediaValue(_rightPin);
}
