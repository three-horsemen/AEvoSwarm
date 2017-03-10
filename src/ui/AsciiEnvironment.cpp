//
// Created by soorya on 5/3/17.
//

#include <ui/AsciiEnvironment.hpp>

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

AsciiEnvironment::AsciiEnvironment(Environment &newEnvironment) : environment(newEnvironment.width,
																			  newEnvironment.height) {
	for (int i = 0; i < newEnvironment.width; i++) {
		for (int j = 0; j < newEnvironment.height; j++) {
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

	for (int i = 0; i < environment.height; i++) {
		cout << "----";
	}
	cout << "-" << endl;
	for (int i = 0; i < environment.height; i++) {
		for (int j = 0; j < environment.width; j++) {
			cout << "| "
				 << ascii_environment::getCharForOccupancy(
						 environment.getTile(Coordinate(j, i)).getAgentCharacter().getOccupancy())
				 << " ";
		}
		cout << "|" << endl;
		for (int j = 0; j < environment.width; j++) {
			cout << "----";
		}
		cout << "-" << endl;
	}
	cout << endl;
}

void AsciiEnvironment::displayPerceptiveField(PerceptiveField &perceptiveField) {

	for (int i = 0; i < perceptiveField.width; i++) {
		cout << "----";
	}
	cout << "-" << endl;
	for (int i = 0; i < perceptiveField.height; i++) {
		for (int j = 0; j < perceptiveField.width; j++) {
			cout << "| "
				 << ascii_environment::getCharForOccupancy(
						 perceptiveField.getTile(Coordinate(j, i)).getAgentCharacter().getOccupancy())
				 << " ";
		}
		cout << "|" << endl;
		for (int j = 0; j < perceptiveField.height; j++) {
			cout << "----";
		}
		cout << "-" << endl;
	}
	cout << endl;
}