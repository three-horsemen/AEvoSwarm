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
				waitKey(1);
				cout << "ants[" << j << "] selected action: " << ants[j].getSelectedAction() << " with potential: "
					 << ants[j].getPotential() << endl;
			}

			ants[j].observeEnvironment(environment);
			ants[j].senseObservation(environment);
			ants[j].selectAction();
			ants[j].performAction((Agent::Action) (selectedAction = (Ant::Action) ants[j].getSelectedAction()));

			if (i % 51 == 0) {
				ants[j].mutate();
			}
//			if (ants[j].getPotential() < 100) {
//				ants[j].setPotential(Ant::HYPOTHETICAL_MAX_ENERGY - 100);
//			}

		}
		Ant::realizeAntsAction(ants, environment);
		//TODO Check for total system conservation
	}
	waitKey();
	return 0;
}