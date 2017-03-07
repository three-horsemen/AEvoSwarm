//
// Created by soorya on 6/3/17.
//

#include "agent/Ant.hpp"
#include "ui/AsciiMap.hpp"

using namespace std;

int main() {
	Attitude randomAttitude = 2;
	Trait randomTrait = 3;
	Occupancy randomOccupancy = OCCUPANCY_WEST;
	Coordinate randomCoordinate(5, 2);
	Energy newPotential = 5;
	Energy newShield = 6;
	Energy newFertility = 7;
	Energy newBaby = 8;
	Ant ant(randomCoordinate, newPotential, newShield, newFertility, newBaby,
			AgentCharacter(randomAttitude, randomTrait, randomOccupancy));
	Map map = Map::generateRandomMap();
	cout << "Total energy present on the random map: " << map.getTotalEnergy() << endl;
	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				map.setTile(ant >> map.getTile(Coordinate(i, j)), Coordinate(i, j));
			}
		}
	}
	int choice;
	do {
		AsciiMap::displayAsciiMap(map);
		cout << "What action to perform? (Press 9 to exit)" << endl <<
			 Ant::FORWARD << ". FORWARD,\n" <<
			 Ant::LEFT << ". LEFT,\n" <<
			 Ant::RIGHT << ". RIGHT\n" <<
			 "::";
		cin >> choice;
		switch (choice) {
			case Ant::FORWARD:
				ant.moveForward(map);
				break;
			case Ant::LEFT:
				ant.turnLeft(map);
				break;
			case Ant::RIGHT:
				ant.turnRight(map);
				break;
			default:
				break;
		}
	} while (choice != 9);
	return 0;
}