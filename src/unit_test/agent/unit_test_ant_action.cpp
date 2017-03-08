//
// Created by reuben on 5/3/17.
//

#include <vector>

#include <ui/AsciiEnvironment.hpp>
#include <agent/Ant.hpp>

int main(int argc, char *argv[]) {

	Coordinate randomCoordinate(5, 2);

	vector<Ant> ants(1);

	ants[0].randomize();
	Environment environment(10, 10);
	environment.randomize();
	ants[0].placeAntInEnvironment(environment, randomCoordinate);

	cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
	for (int i = 0; i < environment.width; i++) {
		for (int j = 0; j < environment.height; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				environment.setTile(ants[0] >> environment.getTile(Coordinate(i, j)), Coordinate(i, j));
			}
		}
	}

	Ant::Action selectedAction;
	do {
		AsciiEnvironment::displayAsciiEnvironment(environment);

		ants[0].observeEnvironment(environment);
		ants[0].senseObservation(environment);
		selectedAction = (Ant::Action) ants[0].getSelectedAction();
		ants[0].performAction((Agent::Action) selectedAction);

		environment.clearCharacterGrid();
		Ant::realizeAntsAction(ants, environment);

	} while (selectedAction != Ant::Action::DIE);
	return 0;
}

