//
// Created by soorya on 6/3/17.
//

#include "agent/Ant.hpp"
#include "ui/AsciiEnvironment.hpp"

using namespace std;

int main() {
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
		ant.observeEnvironment(environment);
		AsciiEnvironment::displayPerceptiveField(*ant.getPerceptiveField());
		ant.performAction((Agent::Action) action);
		AsciiEnvironment::displayPerceptiveField(*ant.getPerceptiveField());
	} while (action != 9);
	return 0;
}