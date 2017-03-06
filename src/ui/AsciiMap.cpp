//
// Created by soorya on 5/3/17.
//

#include <iostream>
#include <agent/PerceptiveField.hpp>
#include "ui/AsciiMap.hpp"

using namespace std;

namespace ascii_map {
	char getCharForOccupancy(Occupancy occupancy) {
		if (occupancy == OCCUPANCY_DEAD)
			return ' ';
		else if (occupancy == OCCUPANCY_NORTH)
			return '^';
		else if (occupancy == OCCUPANCY_SOUTH)
			return 'v';
		else if (occupancy == OCCUPANCY_EAST)
			return '>';
		else if (occupancy == OCCUPANCY_WEST)
			return '<';
	}
}

AsciiMap::AsciiMap(Map &newMap) {
	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
			map.setTile(
					Tile(newMap.getTile(Coordinate(i, j))),
					Coordinate(i, j));
		}
	}
}

Map AsciiMap::getMap() {
	return map;
}

void AsciiMap::setMap(Map newMap) {
	map = newMap;
}

void AsciiMap::displayAsciiMap(Map &map) {

	for (int i = 0; i < Map::MAX_X; i++) {
		cout << "----";
	}
	cout << "-" << endl;
	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
			cout << "| "
				 << ascii_map::getCharForOccupancy(map.getTile(Coordinate(i, j)).getAgentCharacter().getOccupancy())
				 << " ";
		}
		cout << "|" << endl;
		for (int j = 0; j < Map::MAX_Y; j++) {
			cout << "----";
		}
		cout << "-" << endl;
	}
	cout << endl;
}

void AsciiMap::displayPerceptiveField(PerceptiveField &perceptiveField) {

	for (int i = 0; i < PerceptiveField::WIDTH; i++) {
		cout << "----";
	}
	cout << "-" << endl;
	for (int i = 0; i < PerceptiveField::WIDTH; i++) {
		for (int j = 0; j < PerceptiveField::HEIGHT; j++) {
			cout << "| "
				 << ascii_map::getCharForOccupancy(perceptiveField.tile[i][j].getAgentCharacter().getOccupancy())
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