//
// Created by soorya on 10/3/17.
//

#include <ui/OpenCV.hpp>

int main(int argc, char *argv[]) {
	Agent::initialize();

	Coordinate randomCoordinate(5, 2);

	vector<Ant> ants(1);

	ants[0].randomize();
	ants[0].setPotential(Ant::HYPOTHETICAL_MAX_ENERGY - 100);
	Environment environment(80, 80);
//	Environment environment(6, 6);
	environment.randomize();

	for (int i = 0; i < environment.width; i++) {
		for (int j = 0; j < environment.height; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				Ant::placeInEnvironment(ants[0], environment, Coordinate(i, j));
			}
		}
	}
	ui::OpenCV openCVEnvironment(environment);

<<<<<<< HEAD

	for (int i = 0; i < 10000000 && ants.size() > 0; i++) {
		Energy priorEnergy = environment.getTotalEnergy();
//		cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
		unsigned long antCount = ants.size();
		for (unsigned long j = 0; j < antCount; j++) {
			if (i % 1 == 0) {
				openCVEnvironment.displayEnvironment();
=======
	char escapeKey = char(27);
	char pressedKey = 0;
	Ant::Action selectedAction = Ant::Action::FORWARD;
	for (unsigned long long i = 0; i < 10000000 && ants.size() > 0 && (pressedKey != escapeKey); i++) {
//		cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
		unsigned long long antCount = ants.size();
		for (unsigned long long j = 0; j < antCount; j++) {
//			ants[j].setSelectedAction((Agent::Action)Ant::Action::FORTIFY);
			if (i % 1 == 0) {
				pressedKey = openCVEnvironment.displayEnvironment(ants, i);
>>>>>>> 6a639343fb7a5b283ab513ef8f5c8c2100774072
				waitKey(1);
//				cout << "ants[" << j << "] selected action: " << ants[j].getSelectedAction() << " with potential: "<< ants[j].getPotential() << endl;
			}

			ants[j].observeEnvironment(environment);
			ants[j].senseObservation(environment);
			ants[j].selectAction();
			ants[j].performAction((Agent::Action) ((Ant::Action) ants[j].getSelectedAction()));

			if (i % 51 == 0) {
<<<<<<< HEAD
//				ants[j].mutate();
			}
=======
				ants[j].mutate();
			}
//			if (ants[j].getPotential() < 100) {
//				ants[j].setPotential(Ant::HYPOTHETICAL_MAX_ENERGY - 100);
//			}
>>>>>>> 6a639343fb7a5b283ab513ef8f5c8c2100774072

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
	waitKey();
	return 0;
}