//
// Created by soorya on 5/3/17.
//

#include "map/Map.hpp"

Map::Map(Tile newTiles[MAP_MAX_X][MAP_MAX_Y]) {
	for (int i = 0; i < MAP_MAX_X; i++) {
		for (int j = 0; j < MAP_MAX_Y; j++) {
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
