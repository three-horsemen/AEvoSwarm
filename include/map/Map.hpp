//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_MAP_HPP
#define SFTP_MAP_HPP

#include "map/Tile.hpp"

#define MAP_MAX_X 10
#define MAP_MAX_Y 10

class Map {
protected:
	Tile tiles[MAP_MAX_X][MAP_MAX_Y];
public:
	Map() {}

	Map(Tile Tiles[MAP_MAX_X][MAP_MAX_Y]);

	Tile getTile(Coordinate);

	void setTile(Tile, Coordinate);
};

typedef Map Environment;

#endif //SFTP_MAP_HPP
