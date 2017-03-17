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
	const Energy HYPOTHETICAL_MAX_ENERGY = 400;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float r = (rand() % 256) / 255.f;
			tiles[i][j].setAgentCharacter(AgentCharacter());
			tiles[i][j].setGlobalCoordinate(Coordinate(j, i));
			tiles[i][j].setTotalEnergy((Energy) (r * r * HYPOTHETICAL_MAX_ENERGY));
		}
	}
}

void Environment::save(ostream &file) {
	file << height << ' ' << width << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			file << tiles[i][j].getTotalEnergy() << ' '
				 << tiles[i][j].getAgentCharacter().getAttitude() << ' '
				 << tiles[i][j].getAgentCharacter().getOccupancy() << ' '
				 << tiles[i][j].getAgentCharacter().getTrait() << "  ";
		}
		file << endl;
	}
}

bool Environment::load(istream &file) {
	try {
		int fileHeight, fileWidth;
		file >> fileHeight >> fileWidth;
		if (fileHeight != height || fileWidth != width) {
			return false;
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Energy totalEnergy;
				file >> totalEnergy;
				tiles[i][j].setTotalEnergy(totalEnergy);

				AgentCharacter character;

				Attitude attitude;
				file >> attitude;
				character.setAttitude(attitude);

				int occupancy;
				file >> occupancy;
				character.setOccupancy((Occupancy) occupancy);

				Trait trait;
				file >> trait;
				character.setTrait(trait);

				tiles[i][j].setAgentCharacter(character);
			}
		}
		return true;
	} catch (exception &e) {
		cout << e.what() << endl;
		return false;
	}
}