//
// Created by reuben on 5/3/17.
//

#include <ui/AsciiMap.hpp>
#include <agent/Ant.hpp>

int main(int argc, char *argv[]) {

	Attitude randomAttitude = 2;
	Trait randomTrait = 3;
	Occupancy randomOccupancy = OCCUPANCY_DEAD;
	Energy randomEnergy = 10;

	Environment environment;
//	Ant ant(environment);

	Coordinate randomCoordinate(5, 5);
	AgentCharacter randomCharacter(randomAttitude, randomTrait, randomOccupancy);
	Energy newPotential = 5;
	Energy newShield = 6;
	Energy newFertility = 7;
	Energy newBaby = 8;
	Ant ant(randomCoordinate, newPotential, newShield, newFertility, newBaby, randomCharacter);

	for (int i = 0; i < Map::MAX_X; i++) {
		for (int j = 0; j < Map::MAX_Y; j++) {
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
	environment.setTile(Tile(
			Coordinate(0, 0),
			AgentCharacter(Attitude(randomAttitude), Trait(randomTrait), Occupancy(OCCUPANCY_WEST)),
			Energy(randomEnergy)
						),
						Coordinate(0, 1));
	environment.setTile(Tile(
			Coordinate(0, 1),
			AgentCharacter(Attitude(randomAttitude), Trait(randomTrait), Occupancy(OCCUPANCY_EAST)),
			Energy(randomEnergy)
						),
						Coordinate(0, 1));
//	AsciiMap asciiMap(environment);
//	asciiMap.displayAsciiMap();
	AsciiMap::displayAsciiMap(environment);

	ant.observeEnvironment(environment);
	AsciiMap::displayPerceptiveField(*ant.getPerceptiveField());

	return 0;
}

