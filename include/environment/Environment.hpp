//
// Created by soorya on 5/3/17.
//

#ifndef AEVO_ENVIRONMENT_HPP
#define AEVO_ENVIRONMENT_HPP

#include <environment/Tile.hpp>
#include <agent/AgentCharacter.hpp>

#include <shared/Utils.hpp>

#include <vector>
#include <time.h>
#include <fstream>

using namespace std;

namespace adjacency {
	enum Adjacency {
		UNDER,
		AHEAD,
		LEFT,
		RIGHT,
		BEHIND,
		AHEAD_LEFT,
		AHEAD_RIGHT
	};
	const short ADJACENCY_COUNT = 4;
}

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

	static Coordinate getCoordinate(Coordinate, Occupancy, adjacency::Adjacency);

	Coordinate getGlobalCoordinate(Coordinate, Occupancy, adjacency::Adjacency);

	Energy getTotalEnergy();

	void clearCharacterGrid();

	void randomize();

	void save(ostream &);

	bool load(istream &);
};

#endif //AEVO_ENVIRONMENT_HPP
