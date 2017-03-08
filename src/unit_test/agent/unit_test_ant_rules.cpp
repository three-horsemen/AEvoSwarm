//
// Created by reuben on 5/3/17.
//

#include <ui/AsciiEnvironment.hpp>
#include <agent/Ant.hpp>

int main(int argc, char *argv[]) {

	Coordinate randomCoordinate(5, 2);
	Ant ant;
	ant.randomize();
	Environment environment(10, 10);
	environment.randomize();
	ant.placeAntInEnvironment(environment, randomCoordinate);
	cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
	for (int i = 0; i < environment.width; i++) {
		for (int j = 0; j < environment.height; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				environment.setTile(ant >> environment.getTile(Coordinate(i, j)), Coordinate(i, j));
			}
		}
	}

	Ant::Action selectedAction;
	do {
		AsciiEnvironment::displayAsciiEnvironment(environment);

		ant.observeEnvironment(environment);
		selectedAction = (Ant::Action) ant.getSelectAction();
		ant.performAction((Agent::Action) selectedAction);
		ant.affectEnvironment(environment);

	} while (selectedAction != Ant::Action::DIE);
	return 0;
}

