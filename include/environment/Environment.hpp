//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_ENVIRONMENT_HPP
#define SFTP_ENVIRONMENT_HPP

#include "environment/Tile.hpp"
#include <time.h>

class Environment {
public:
	static const short MAX_X = 10;
	static const short MAX_Y = 10;
protected:
	Tile tiles[MAX_X][MAX_X];
public:
	Environment();

	Environment(Tile Tiles[MAX_X][MAX_X]);

	Tile getTile(Coordinate);

	void setTile(Tile, Coordinate);

	Energy getTotalEnergy();

	static Environment generateRandomEnvironment();
};

#endif //SFTP_ENVIRONMENT_HPP
