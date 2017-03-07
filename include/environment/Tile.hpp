//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_TILE_HPP
#define SFTP_TILE_HPP

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

	Coordinate getCoordinate();

	AgentCharacter getAgentCharacter();

	Energy getTotalEnergy();

	void setCoordinate(Coordinate);

	void setAgentCharacter(AgentCharacter);

	void setTotalEnergy(Energy);
};

#endif //SFTP_TILE_HPP
