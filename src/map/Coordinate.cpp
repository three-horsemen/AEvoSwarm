//
// Created by soorya on 5/3/17.
//

#include "map/Coordinate.hpp"

//Coordinate::Coordinate(Coordinate &newCoordinate) {
//	X = newCoordinate.getX();
//	Y = newCoordinate.getY();
//}

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