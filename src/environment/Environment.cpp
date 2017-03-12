//
// Created by soorya on 5/3/17.
//

#include "environment/Environment.hpp"

Environment::Environment(const Environment &environment) :
		_width(environment.width), _height(environment.height),
		width(_width), height(_height) {
	operator=(environment);
}

void Environment::operator=(const Environment &environment) {
	_width = environment.width;
	_height = environment._height;
	for (int i = 0; i < tiles.size(); i++)
		tiles[i].clear();
	tiles.clear();

	for (int i = 0; i < height; i++) {
		vector<Tile> row;
		for (int j = 0; j < width; j++) {
			row.push_back(environment.getTile(Coordinate(j, i)));
		}
		tiles.push_back(row);
	}
}

Environment::Environment(short width, short height) : _width(width), _height(height), width(_width), height(_height) {
	for (int i = 0; i < height; i++) {
		vector<Tile> row;
		for (int j = 0; j < width; j++) {
			Tile tile;
			tile.setGlobalCoordinate(Coordinate(j, i));
			row.push_back(tile);
		}
		tiles.push_back(row);
	}
}

Tile Environment::getTile(Coordinate coordinate) const {
	return tiles[coordinate.getY()][coordinate.getX()];
}

void Environment::setTile(Tile newTile, Coordinate newCoordinate) {
	tiles[newCoordinate.getY()][newCoordinate.getX()] = newTile;
}

Energy Environment::getTotalEnergy() {
	Energy totalEnergy = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			totalEnergy += tiles[i][j].getTotalEnergy();
		}
	}
	return totalEnergy;
}

void Environment::clearCharacterGrid() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Tile tile = getTile(Coordinate(i, j));
			tile.setAgentCharacter(AgentCharacter());
			setTile(tile, Coordinate(i, j));
		}
	}
}

void Environment::randomize() {
	const Energy HYPOTHETICAL_MAX_ENERGY = 500;
	srand((unsigned int) time(NULL));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			tiles[i][j].setAgentCharacter(AgentCharacter());
			tiles[i][j].setGlobalCoordinate(Coordinate(j, i));
			tiles[i][j].setTotalEnergy((Energy) (rand() % HYPOTHETICAL_MAX_ENERGY));
		}
	}
}