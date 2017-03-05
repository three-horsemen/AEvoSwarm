//
// Created by soorya on 5/3/17.
//

#include <iostream>
#include "ui/AsciiMap.hpp"

using namespace std;

AsciiMap::AsciiMap(Map &newMap) {
	for (int i = 0; i < MAP_MAX_X; i++) {
		for (int j = 0; j < MAP_MAX_Y; j++) {
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

void AsciiMap::displayAsciiMap() {

	for (int i = 0; i < MAP_MAX_X; i++) {
		cout << "#";
	}
	cout << endl;
	for (int i = 0; i < MAP_MAX_X; i++) {
		for (int j = 0; j < MAP_MAX_Y; j++) {
			Coordinate tempCoordinate(i, j);
			cout << charOccupancy[map.getTile(tempCoordinate).getAntCharacter().getOccupancy()];
		}
		cout << endl;
	}
	for (int i = 0; i < MAP_MAX_X; i++) {
		cout << "#";
	}
	cout << endl;
}