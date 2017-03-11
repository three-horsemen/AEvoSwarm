//
// Created by soorya on 10/3/17.
//

#include <ui/OpenCV.hpp>
#include <agent/Ant.hpp>

int main(int argc, char *argv[]) {
	Agent::initialize();

	Coordinate randomCoordinate(5, 2);

	vector<Ant> ants(1);

	ants[0].randomize();
	ants[0].setPotential(Ant::HYPOTHETICAL_MAX_ENERGY - 100);
	Environment environment(20, 20);
	environment.randomize();

	for (int i = 0; i < environment.width; i++) {
		for (int j = 0; j < environment.height; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				Ant::placeInEnvironment(ants[0], environment, Coordinate(i, j));
			}
		}
	}
	ui::OpenCV openCVEnvironment(environment);


	for (int i = 0; i < 10000000 && ants.size() > 0; i++) {
		Energy priorEnergy = environment.getTotalEnergy();
//		cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
		unsigned long antCount = ants.size();
		for (unsigned long j = 0; j < antCount; j++) {
			if (i % 1 == 0) {
				openCVEnvironment.displayEnvironment();
				waitKey(1);
//				cout << "ants[" << j << "] selected action: " << ants[j].getSelectedAction() << " with potential: "<< ants[j].getPotential() << endl;
			}

			ants[j].observeEnvironment(environment);
			ants[j].senseObservation(environment);
			ants[j].selectAction();
			ants[j].performAction((Agent::Action) ((Ant::Action) ants[j].getSelectedAction()));

			if (i % 51 == 0) {
//				ants[j].mutate();
			}

		}

		Ant::realizeAntsAction(ants, environment);

		if (ants.size() < 15) {
			int randXOffset = rand() % environment.width;
			int randYOffset = rand() % environment.height;
			for (int x = 0; x < environment.width; x++) {
				for (int y = 0; y < environment.height; y++) {
					int X = (x + randXOffset) % environment.width;
					int Y = (y + randYOffset) % environment.height;

					Tile tile = environment.getTile(Coordinate(X, Y));
					if (tile.getAgentCharacter().getOccupancy() ==
						OCCUPANCY_DEAD && !Ant::isInImpactRange(environment, tile.getGlobalCoordinate()) &&
						tile.getTotalEnergy() >= Ant::NEWBORN_MIN_TOTAL_ENERGY) {

						Ant ant;

						ant.randomize();
						ant.setShield(Ant::NEWBORN_MIN_SHIELD);
						ant.setFetal(0);
						ant.setFertility(0);
						ant.setPushedFetalEnergy(0);
						Ant::placeCharacterInEnvironment(ant, environment, Coordinate(X, Y));

						ant.mutate();
						ants.push_back(ant);
						tile.setTotalEnergy(0);
						cout << "Spawned ant at (" << X << "," << Y << ")" << endl;

					}
				}
			}
		}
		if (environment.getTotalEnergy() != priorEnergy) {
			cout << "Environment energy is not conserved from " << priorEnergy << " to " << environment.getTotalEnergy()
				 << endl;
		}
		//TODO Check for total system conservation
	}
	return 0;
}