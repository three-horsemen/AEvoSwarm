//
// Created by soorya on 5/3/17.
//

#ifndef AEVO_ENVIRONMENT_HPP
#define AEVO_ENVIRONMENT_HPP

#include <environment/Tile.hpp>
#include <agent/AgentCharacter.hpp>

#include <vector>
#include <time.h>
#include <fstream>

using namespace std;

class Environment {
protected:
	short _width, _height;
	vector<vector<Tile> > tiles;
public:
	short &width, &height;

	Environment(const Environment &);

	void operator=(const Environment &);

	Environment(short, short);

	Tile getTile(Coordinate) const;

	void setTile(Tile, Coordinate);

	Energy getTotalEnergy();

	void clearCharacterGrid();

	void randomize();

	void save(string);

	bool load(string);
};

#endif //AEVO_ENVIRONMENT_HPP
