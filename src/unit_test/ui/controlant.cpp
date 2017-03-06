//
// Created by soorya on 6/3/17.
//

#include "agent/Ant.hpp"
#include "ui/AsciiMap.hpp"

using namespace std;

void moveAnt(Map &map, Ant &ant, Coordinate destination) {
	map.setTile(
			(ant << map.getTile(ant.getCoordinate())),
			ant.getCoordinate()
	);

	map.setTile(
			(ant >> map.getTile(destination)),
			destination
	);
}

void moveAnt(Map &map, Ant &ant) {
	Coordinate destinationCoordinate = ant.getCoordinate();
	Occupancy occupancy = ant.getCharacter().getOccupancy();
	cout << "Direction of movement: " << occupancy << endl;
	if (occupancy == OCCUPANCY_NORTH)
		destinationCoordinate.setX(Utils::modulo((destinationCoordinate.getX() - 1), MAP_MAX_X));
	else if (occupancy == OCCUPANCY_EAST)
		destinationCoordinate.setY(Utils::modulo((destinationCoordinate.getY() + 1), MAP_MAX_Y));
	else if (occupancy == OCCUPANCY_SOUTH)
		destinationCoordinate.setX(Utils::modulo((destinationCoordinate.getX() + 1), MAP_MAX_X));
	else if (occupancy == OCCUPANCY_WEST)
		destinationCoordinate.setY(Utils::modulo((destinationCoordinate.getY() - 1), MAP_MAX_Y));
	cout << "Destination coordinate: (" << destinationCoordinate.getX() << " " << destinationCoordinate.getY() << ")"
		 << endl;
	moveAnt(map, ant, destinationCoordinate);
}

int main() {
	Attitude randomAttitude = 2;
	Trait randomTrait = 3;
	Occupancy randomOccupancy = OCCUPANCY_WEST;
	Energy randomEnergy = 10;
	Coordinate randomCoordinate(5, 2);
	Map map;

	Energy newPotential = 5;
	Energy newShield = 6;
	Energy newFertility = 7;
	Energy newBaby = 8;
	Ant ant(randomCoordinate, newPotential, newShield, newFertility, newBaby,
			AgentCharacter(randomAttitude, randomTrait, randomOccupancy));
	for (int i = 0; i < MAP_MAX_X; i++) {
		for (int j = 0; j < MAP_MAX_Y; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				map.setTile(ant >> map.getTile(Coordinate(i, j)), Coordinate(i, j));
			}
		}
	}
//	AsciiMap asciiMap(map);
	AsciiMap::displayAsciiMap(map);

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
				moveAnt(map, ant);
				break;
		}
		AsciiMap::displayAsciiMap(map);
	} while (choice != 9);
	return 0;
}