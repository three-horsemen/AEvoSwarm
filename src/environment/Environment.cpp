//
// Created by soorya on 5/3/17.
//

#include "environment/Environment.hpp"

Environment::Environment(Environment &environment) :
		_width(environment.width), _height(environment.height),
		width(_width), height(_height) {
	for (int i = 0; i < height; i++) {
		vector<Tile> row;
		for (int j = 0; j < width; j++) {
			row.push_back(environment.getTile(Coordinate(j, i)));
		}
		tiles.push_back(row);
	}
}

void Environment::operator=(Environment &environment) {
	_width = environment.width;
	_height = environment._height;
	for (int i = 0; i < height; i++)
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
			tile.setCoordinate(Coordinate(j, i));
			row.push_back(tile);
		}
		tiles.push_back(row);
	}
}

Tile Environment::getTile(Coordinate coordinate) {
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
	//TODO Implement stub
}

void Environment::randomize() {
	const Energy HYPOTHETICAL_MAX_ENERGY = 10;
	srand((unsigned int) time(NULL));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			tiles[i][j].setAgentCharacter(AgentCharacter());
			tiles[i][j].setCoordinate(Coordinate(j, i));
			tiles[i][j].setTotalEnergy((Energy) (rand() % HYPOTHETICAL_MAX_ENERGY));
		}
	}
}