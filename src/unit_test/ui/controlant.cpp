//
// Created by soorya on 6/3/17.
//

#include "ui/AsciiEnvironment.hpp"
#include <agent/Ant.hpp>

using namespace std;

int main() {
	srand((unsigned int) time(NULL));

	vector<Ant> ants(1);

	ants[0].randomize();
	Environment environment(10, 10);
	environment.randomize();
	Coordinate randomCoordinate(5, 2);

	cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
	for (int i = 0; i < environment.width; i++) {
		for (int j = 0; j < environment.height; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				ants[0].placeInEnvironment(ants[0], environment, randomCoordinate);
			}
		}
	}
	Ant::Action action;
	do {
		AsciiEnvironment::displayAsciiEnvironment(environment);
		cout << "What action to perform? (Press 9 to exit)" << endl <<
			 Ant::Action::FORWARD << ". FORWARD,\n" <<
			 Ant::Action::LEFT << ". LEFT,\n" <<
			 Ant::Action::RIGHT << ". RIGHT\n" <<
			 "::";
		int choice;
		cin >> choice;
		action = (Ant::Action) choice;
		ants[0].observeEnvironment(environment);
		ants[0].performAction((Agent::Action) action);
		Ant::realizeAntsAction(ants, environment);
	} while (action != 9);
	return 0;
}