//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_ENVIRONMENT_HPP
#define SFTP_ENVIRONMENT_HPP

#include "environment/Tile.hpp"

#include <vector>
#include <time.h>

using namespace std;

class Environment {
protected:
	short _width, _height;
	vector<vector<Tile> > tiles;
public:
	short &width, &height;

	Environment(Environment &);

	void operator=(Environment &);

	Environment(short, short);

	Tile getTile(Coordinate);

	void setTile(Tile, Coordinate);

	Energy getTotalEnergy();

	void clearCharacterGrid();

	void randomize();
};

#endif //SFTP_ENVIRONMENT_HPP
