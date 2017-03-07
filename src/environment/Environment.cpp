//
// Created by soorya on 5/3/17.
//

#include "environment/Environment.hpp"

Environment::Environment() {
	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			tiles[i][j].setCoordinate(Coordinate(i, j));
		}
	}
}

Environment::Environment(Tile newTiles[Environment::MAX_X][Environment::MAX_Y]) {
	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			tiles[i][j] = newTiles[i][j];
		}
	}
}

Tile Environment::getTile(Coordinate coordinate) {
	return tiles[coordinate.getX()][coordinate.getY()];
}

void Environment::setTile(Tile newTile, Coordinate newCoordinate) {
	tiles[newCoordinate.getX()][newCoordinate.getY()] = newTile;
}

Energy Environment::getTotalEnergy() {
	Energy totalEnergy = 0;
	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			totalEnergy += tiles[i][j].getTotalEnergy();
		}
	}
	return totalEnergy;
}

Environment Environment::generateRandomEnvironment() {
	Environment map;
	const Energy HYPOTHETICAL_MAX_ENERGY = 10;
	srand((unsigned int) time(NULL));
	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			map.tiles[i][j].setAgentCharacter(AgentCharacter());
			map.tiles[i][j].setCoordinate(Coordinate(i, j));
			map.tiles[i][j].setTotalEnergy((Energy) (rand() % HYPOTHETICAL_MAX_ENERGY));
		}
	}
	return map;
}