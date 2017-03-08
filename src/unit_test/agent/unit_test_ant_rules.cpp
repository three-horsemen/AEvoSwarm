//
// Created by reuben on 5/3/17.
//

#include <ui/AsciiEnvironment.hpp>
#include <agent/Ant.hpp>

int main(int argc, char *argv[]) {

	Attitude randomAttitude = 2;
	Trait randomTrait = 3;
	Occupancy randomOccupancy = OCCUPANCY_DEAD;
	Energy randomEnergy = 10;

	Environment environment;
//	Ant ant(environment);

	Coordinate randomCoordinate(2, 2);
	AgentCharacter randomCharacter(randomAttitude, randomTrait, randomOccupancy);
	Energy newPotential = 5;
	Energy newShield = 6;
	Energy newFertility = 7;
	Energy newBaby = 8;
	Ant ant(randomCoordinate, newPotential, newShield, newFertility, newBaby, AgentCharacter(0, 0, OCCUPANCY_NORTH));

	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			environment.setTile(
					Tile(
							Coordinate(i, j),
							randomCharacter,
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
	AsciiEnvironment::displayAsciiEnvironment(environment);

	ant.observeEnvironment(environment);
	AsciiEnvironment::displayPerceptiveField(*ant.getPerceptiveField());

	return 0;
}

