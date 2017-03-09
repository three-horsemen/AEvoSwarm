//
// Created by soorya on 5/3/17.
//

#ifndef AEVO_TILE_HPP
#define AEVO_TILE_HPP

#include "environment/Coordinate.hpp"
#include "agent/AgentCharacter.hpp"
#include "environment/Energy.hpp"
#include <iostream>
using namespace std;

class Tile {
protected:
	Coordinate coordinate;
	AgentCharacter agentCharacter;
	Energy totalEnergy;
public:
	Tile();

	Tile(Coordinate, AgentCharacter, Energy);

	Coordinate getGlobalCoordinate();

	AgentCharacter getAgentCharacter();

	Energy getTotalEnergy();

	void setGlobalCoordinate(Coordinate);

	void setAgentCharacter(AgentCharacter);

	void setTotalEnergy(Energy);
};

#endif //AEVO_TILE_HPP
