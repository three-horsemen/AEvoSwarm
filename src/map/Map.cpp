//
// Created by soorya on 5/3/17.
//

#include "map/Map.hpp"

Map::Map() {
	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
			tiles[i][j].setCoordinate(Coordinate(i, j));
		}
	}
}

Map::Map(Tile newTiles[Map::MAX_X][Map::MAX_Y]) {
	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
			tiles[i][j] = newTiles[i][j];
		}
	}
}

Tile Map::getTile(Coordinate coordinate) {
	return tiles[coordinate.getX()][coordinate.getY()];
}

void Map::setTile(Tile newTile, Coordinate newCoordinate) {
	tiles[newCoordinate.getX()][newCoordinate.getY()] = newTile;
}

Energy Map::getTotalEnergy() {
	Energy totalEnergy = 0;
	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
			totalEnergy += tiles[i][j].getTotalEnergy();
		}
	}
	return totalEnergy;
}

Map Map::generateRandomMap() {
	Map map;
	const Energy HYPOTHETICAL_MAX_ENERGY = 10;
	srand((unsigned int) time(NULL));
	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
			map.tiles[i][j].setAgentCharacter(AgentCharacter());
			map.tiles[i][j].setCoordinate(Coordinate(i, j));
			map.tiles[i][j].setTotalEnergy((Energy) (rand() % HYPOTHETICAL_MAX_ENERGY));
		}
	}
	return map;
}