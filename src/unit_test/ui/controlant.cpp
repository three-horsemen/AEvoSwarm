//
// Created by soorya on 6/3/17.
//

#include "agent/Ant.hpp"
#include "ui/AsciiEnvironment.hpp"

using namespace std;

int main() {
	Coordinate randomCoordinate(5, 2);
	Ant ant = Ant::generateRandomAnt();
	Environment environment = Environment::generateRandomEnvironment();
	ant.placeAntInEnvironment(environment, randomCoordinate);
	cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
	for (int i = 0; i < Environment::MAX_X; i++) {
		for (int j = 0; j < Environment::MAX_Y; j++) {
			if (randomCoordinate == Coordinate(i, j)) {
				environment.setTile(ant >> environment.getTile(Coordinate(i, j)), Coordinate(i, j));
			}
		}
	}
	int choice;
	do {
		AsciiEnvironment::displayAsciiEnvironment(environment);
		cout << "What action to perform? (Press 9 to exit)" << endl <<
			 ant::action::FORWARD << ". FORWARD,\n" <<
			 ant::action::LEFT << ". LEFT,\n" <<
			 ant::action::RIGHT << ". RIGHT\n" <<
			 "::";
		cin >> choice;
		switch (choice) {
			case ant::action::FORWARD:
				ant.moveForward(environment);
				break;
			case ant::action::LEFT:
				ant.turnLeft(environment);
				break;
			case ant::action::RIGHT:
				ant.turnRight(environment);
				break;
			default:
				break;
		}
	} while (choice != 9);
	return 0;
}