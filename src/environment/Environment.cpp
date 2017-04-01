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

Coordinate Environment::getCoordinate(Coordinate coordinate, Occupancy occupancy, adjacency::Adjacency adjacency) {
	if (adjacency == adjacency::UNDER)
		return coordinate;
	int x = coordinate.getX(), y = coordinate.getY();
	switch (occupancy) {
		case OCCUPANCY_NORTH:
			switch (adjacency) {
				case adjacency::AHEAD:
					y--;
					break;
				case adjacency::BEHIND:
					y++;
					break;
				case adjacency::LEFT:
					x--;
					break;
				case adjacency::RIGHT:
					x++;
					break;
				case adjacency::AHEAD_LEFT:
					y--;
					x--;
					break;
				case adjacency::AHEAD_RIGHT:
					y--;
					x++;
					break;
				default:
					throw invalid_argument("Unknown adjacency");
			}
			break;
		case OCCUPANCY_SOUTH:
			switch (adjacency) {
				case adjacency::AHEAD:
					y++;
					break;
				case adjacency::BEHIND:
					y--;
					break;
				case adjacency::LEFT:
					x++;
					break;
				case adjacency::RIGHT:
					x--;
					break;
				case adjacency::AHEAD_LEFT:
					y++;
					x++;
					break;
				case adjacency::AHEAD_RIGHT:
					y++;
					x--;
					break;
				default:
					throw invalid_argument("Unknown adjacency");
			}
			break;
		case OCCUPANCY_EAST:
			switch (adjacency) {
				case adjacency::AHEAD:
					x++;
					break;
				case adjacency::BEHIND:
					x--;
					break;
				case adjacency::LEFT:
					y--;
					break;
				case adjacency::RIGHT:
					y++;
					break;
				case adjacency::AHEAD_LEFT:
					x++;
					y--;
					break;
				case adjacency::AHEAD_RIGHT:
					x++;
					y++;
					break;
				default:
					throw invalid_argument("Unknown adjacency");
			}
			break;
		case OCCUPANCY_WEST:
			switch (adjacency) {
				case adjacency::AHEAD:
					x--;
					break;
				case adjacency::BEHIND:
					x++;
					break;
				case adjacency::LEFT:
					y++;
					break;
				case adjacency::RIGHT:
					y--;
					break;
				case adjacency::AHEAD_LEFT:
					x--;
					y++;
					break;
				case adjacency::AHEAD_RIGHT:
					x--;
					y--;
					break;
				default:
					throw invalid_argument("Unknown adjacency");
			}
			break;
		default:
			throw invalid_argument("Unknown occupancy specified");
	}
	return Coordinate(x, y);
}

Coordinate Environment::getGlobalCoordinate(Coordinate coordinate, Occupancy occupancy,
											adjacency::Adjacency adjacency) {
	Coordinate potentialOutOfBoundsCoordinate = Environment::getCoordinate(coordinate, occupancy, adjacency);
	int x = potentialOutOfBoundsCoordinate.getX();
	int y = potentialOutOfBoundsCoordinate.getY();
	potentialOutOfBoundsCoordinate.setX(Utils::modulo(x, width));
	potentialOutOfBoundsCoordinate.setY(Utils::modulo(y, height));
	return potentialOutOfBoundsCoordinate;
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
	const Energy HYPOTHETICAL_MAX_ENERGY = 800;
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
	file <= height <= width;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			file <= tiles[i][j].getTotalEnergy();
			file <= tiles[i][j].getAgentCharacter().getAttitude();
			file <= tiles[i][j].getAgentCharacter().getOccupancy();
			file <= tiles[i][j].getAgentCharacter().getTrait();
		}
	}
}

bool Environment::load(istream &file) {
	try {
		unsigned short fileHeight, fileWidth;
		file >= fileHeight >= fileWidth;
		if (fileHeight != height || fileWidth != width) {
			return false;
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Energy totalEnergy;
				file >= totalEnergy;
				tiles[i][j].setTotalEnergy(totalEnergy);

				AgentCharacter character;

				Attitude attitude;
				file >= attitude;
				character.setAttitude(attitude);

				Occupancy occupancy;
				file >= occupancy;
				character.setOccupancy((Occupancy) occupancy);

				Trait trait;
				file >= trait;
				character.setTrait(trait);

				tiles[i][j].setAgentCharacter(character);
			}
		}
		return true;
	} catch (exception &e) {
		cerr << e.what() << endl;
		return false;
	}
}