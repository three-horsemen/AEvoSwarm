//
// Created by soorya on 5/3/17.
//

#include <iostream>
#include <agent/PerceptiveField.hpp>
#include "ui/AsciiEnvironment.hpp"

using namespace std;

namespace ascii_environment {
	char getCharForOccupancy(Occupancy occupancy) {
		if (occupancy == OCCUPANCY_DEAD)
			return ' ';
		else if (occupancy == OCCUPANCY_NORTH)
			return '^';
		else if (occupancy == OCCUPANCY_SOUTH)
			return 'v';
		else if (occupancy == OCCUPANCY_EAST)
			return '>';
		else //if (occupancy == OCCUPANCY_WEST)
			return '<';
	}
}

AsciiEnvironment::AsciiEnvironment(Environment &newEnvironment) {
	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			environment.setTile(
					Tile(newEnvironment.getTile(Coordinate(i, j))),
					Coordinate(i, j));
		}
	}
}

Environment AsciiEnvironment::getEnvironment() {
	return environment;
}

void AsciiEnvironment::setEnvironment(Environment newEnvironment) {
	environment = newEnvironment;
}

void AsciiEnvironment::displayAsciiEnvironment(Environment &environment) {

	for (int i = 0; i < Environment::MAX_X; i++) {
		cout << "----";
	}
	cout << "-" << endl;
	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			cout << "| "
				 << ascii_environment::getCharForOccupancy(
						 environment.getTile(Coordinate(i, j)).getAgentCharacter().getOccupancy())
				 << " ";
		}
		cout << "|" << endl;
		for (int j = 0; j < Environment::MAX_Y; j++) {
			cout << "----";
		}
		cout << "-" << endl;
	}
	cout << endl;
}

void AsciiEnvironment::displayPerceptiveField(PerceptiveField &perceptiveField) {

	for (int i = 0; i < PerceptiveField::WIDTH; i++) {
		cout << "----";
	}
	cout << "-" << endl;
	for (int i = 0; i < PerceptiveField::WIDTH; i++) {
		for (int j = 0; j < PerceptiveField::HEIGHT; j++) {
			cout << "| "
				 << ascii_environment::getCharForOccupancy(
						 perceptiveField.tile[i][j].getAgentCharacter().getOccupancy())
				 << " ";
		}
		cout << "|" << endl;
		for (int j = 0; j < PerceptiveField::HEIGHT; j++) {
			cout << "----";
		}
		cout << "-" << endl;
	}
	cout << endl;
}