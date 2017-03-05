//
// Created by soorya on 6/3/17.
//

#include "agent/Ant.hpp"
#include "ui/AsciiMap.hpp"
#include <iostream>

using namespace std;

void moveAnt(Map &map, Coordinate source, Coordinate destination) {
	Tile sourceTile = map.getTile(source);
	Tile destinationTile = map.getTile(destination);

	AgentCharacter agentCharacter = sourceTile.getAgentCharacter();
	sourceTile.setAgentCharacter(AgentCharacter());
	destinationTile.setAgentCharacter(agentCharacter);

//	sourceTile.setTotalEnergy()

//	map.setTile(destination, )
}

int main() {
	Attitude randomAttitude = 2;
	Trait randomTrait = 3;
	Occupancy randomOccupancy = OCCUPANCY_WEST;
	Energy randomEnergy = 10;
	Coordinate randomCoordinate(5, 5);

	Map map;
	for (int i = 0; i < MAP_MAX_X; i++) {
		for (int j = 0; j < MAP_MAX_Y; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				Tile tempTile(
						Coordinate(i, j),
						AgentCharacter(Attitude(randomAttitude), Trait(randomTrait), Occupancy(randomOccupancy)),
						Energy(randomEnergy)
				);
				map.setTile(
						tempTile,
						Coordinate(i, j)
				);
			}
		}
	}
	AsciiMap asciiMap(map);
	asciiMap.displayAsciiMap();

	int choice;
	do {
		cout << "What action to perform? " << endl <<
			 Ant::FORWARD << ". FORWARD,\n" <<
			 Ant::LEFT << ". LEFT,\n" <<
			 Ant::RIGHT << ". RIGHT\n" <<
			 "::";
		cin >> choice;
		switch (choice) {
			case Ant::FORWARD:
				break;
		}
	} while (choice != 9);
}