//
// Created by reuben on 5/3/17.
//

#include <vector>

#include <ui/AsciiEnvironment.hpp>
#include <agent/Ant.hpp>

int main(int argc, char *argv[]) {
	srand((unsigned int) time(NULL));

	Coordinate randomCoordinate(5, 2);

	vector<Ant> ants(1);

	ants[0].randomize();
	Environment environment(10, 10);
	environment.randomize();

	for (int i = 0; i < environment.width; i++) {
		for (int j = 0; j < environment.height; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				Ant::placeInEnvironment(ants[0], environment, Coordinate(i, j));
			}
		}
	}

	Ant::Action selectedAction;
	do {
		cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
		AsciiEnvironment::displayAsciiEnvironment(environment);

		ants[0].observeEnvironment(environment);
		ants[0].senseObservation(environment);
		ants[0].selectAction();
		selectedAction = (Ant::Action) ants[0].getSelectedAction();
		ants[0].performAction((Agent::Action) selectedAction);


		Ant::realizeAntsAction(ants, environment);

	} while (selectedAction != Ant::Action::DIE);//TODO Change this condition when there are multiple ants
	return 0;
}

