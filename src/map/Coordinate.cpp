//
// Created by soorya on 5/3/17.
//

#include "map/Coordinate.hpp"

Coordinate::Coordinate() {
	X = 0;
	Y = 0;
}

Coordinate::Coordinate(int newX, int newY) {
	X = newX;
	Y = newY;
}

int Coordinate::getX() const {
	return X;
}

int Coordinate::getY() const {
	return Y;
}

void Coordinate::setX(int newX) {
	X = newX;
}

void Coordinate::setY(int newY) {
	Y = newY;
}

bool Coordinate::operator==(Coordinate coordinate2) {
	return ((coordinate2.getX() == X) && (coordinate2.getY() == Y));
}

bool Coordinate::operator!=(Coordinate coordinate2) {
	return ((coordinate2.getX() != X) || (coordinate2.getY() != Y));
}