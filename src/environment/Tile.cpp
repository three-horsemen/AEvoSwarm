//
// Created by soorya on 5/3/17.
//

#include "environment/Tile.hpp"

Tile::Tile() {
	totalEnergy = 0;
}

Tile::Tile(Coordinate newCoordinate, AgentCharacter newAgentCharacter, Energy newTotalEnergy) :
		coordinate(newCoordinate.getX(), newCoordinate.getY()),
		agentCharacter(newAgentCharacter.getAttitude(), newAgentCharacter.getTrait(),
					   newAgentCharacter.getOccupancy()) {
	coordinate = newCoordinate;
	agentCharacter = newAgentCharacter;
	totalEnergy = newTotalEnergy;
}

Coordinate Tile::getCoordinate() {
	return coordinate;
}

AgentCharacter Tile::getAgentCharacter() {
	return agentCharacter;
}

Energy Tile::getTotalEnergy() {
	return totalEnergy;
}

void Tile::setAgentCharacter(AgentCharacter newAgentCharacter) {
	agentCharacter.setAttitude(newAgentCharacter.getAttitude());
	agentCharacter.setOccupancy(newAgentCharacter.getOccupancy());
	agentCharacter.setTrait(newAgentCharacter.getTrait());
}

void Tile::setCoordinate(Coordinate newCoordinate) {
	coordinate = newCoordinate;
}

void Tile::setTotalEnergy(Energy newTotalEnergy) {
	totalEnergy = newTotalEnergy;
}
