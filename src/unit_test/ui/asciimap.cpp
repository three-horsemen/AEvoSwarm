//
// Created by soorya on 5/3/17.
//

#include "ui/AsciiMap.hpp"

int main() {
	Attitude randomAttitude = 2;
	Trait randomTrait = 3;
	Occupancy randomOccupancy = OCCUPANCY_EAST;
	Energy randomEnergy = 10;
	Map map;
	for (int i = 0; i < MAP_MAX_X; i++) {
		for (int j = 0; j < MAP_MAX_Y; j++) {
			map.setTile(
					Tile(
							Coordinate(i, j),
							AgentCharacter(Attitude(randomAttitude), Trait(randomTrait), Occupancy(randomOccupancy)),
							Energy(randomEnergy)
					),
					Coordinate(i, j)
			);
		}
	}
	AsciiMap asciiMap(map);
	asciiMap.displayAsciiMap();

	return 0;
}