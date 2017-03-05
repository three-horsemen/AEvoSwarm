//
// Created by soorya on 5/3/17.
//

#include "map/Tile.hpp"

//Tile::Tile(Tile &newTile) :
//		coordinate(newTile.getCoordinate().getX(), newTile.getCoordinate().getY()),
//		agentCharacter(newTile.getAgentCharacter().getAttitude(), newTile.getAgentCharacter().getTrait(),
//					 newTile.getAgentCharacter().getOccupancy()) {
//	coordinate = newTile.getCoordinate();
//	agentCharacter = newTile.getAgentCharacter();
//	totalEnergy = newTile.getTotalEnergy();
//}

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
	agentCharacter = newAgentCharacter;
}

void Tile::setCoordinate(Coordinate newCoordinate) {
	coordinate = newCoordinate;
}

void Tile::setTotalEnergy(Energy newTotalEnergy) {
	totalEnergy = newTotalEnergy;
}