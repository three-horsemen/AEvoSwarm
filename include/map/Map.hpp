//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_MAP_HPP
#define SFTP_MAP_HPP

#include "map/Tile.hpp"
#include <time.h>

class Map {
public:
	static const short MAX_X = 10;
	static const short MAX_Y = 10;
protected:
	Tile tiles[MAX_X][MAX_X];
public:
	Map();

	Map(Tile Tiles[MAX_X][MAX_X]);

	Tile getTile(Coordinate);

	void setTile(Tile, Coordinate);

	Energy getTotalEnergy();

	static Map generateRandomMap();
};

typedef Map Environment;


#endif //SFTP_MAP_HPP
