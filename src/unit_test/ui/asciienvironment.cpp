//
// Created by soorya on 5/3/17.
//

#include "ui/AsciiEnvironment.hpp"

int main() {
	Attitude randomAttitude = 2;
	Trait randomTrait = 3;
	Occupancy randomOccupancy = OCCUPANCY_EAST;
	Energy randomEnergy = 10;
	Environment environment(10, 10);
	for (int i = 0; i < environment.width; i++) {
		for (int j = 0; j < environment.height; j++) {
			environment.setTile(
					Tile(
							Coordinate(i, j),
							AgentCharacter(Attitude(randomAttitude), Trait(randomTrait), Occupancy(randomOccupancy)),
							Energy(randomEnergy)
					),
					Coordinate(i, j)
			);
		}
	}
	AsciiEnvironment::displayAsciiEnvironment(environment);
	return 0;
}