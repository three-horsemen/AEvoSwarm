//
// Created by soorya on 10/3/17.
//

#include <ui/OpenCV.hpp>
#include <agent/Ant.hpp>

int main() {

	Agent::initialize();

	Coordinate randomCoordinate(5, 2);

	vector<Ant> ants(1);

	ants[0].randomize();
	ants[0].setPotential(30000);
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


	Ant::Action selectedAction;
	do {
		cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
		openCVEnvironment.displayEnvironment();
		waitKey();
		ants[0].observeEnvironment(environment);
		ants[0].senseObservation(environment);
		ants[0].selectAction();
		cout << "Ant[0] selected action: " << ants[0].getSelectedAction() << endl;
		selectedAction = (Ant::Action) ants[0].getSelectedAction();
		ants[0].performAction((Agent::Action) selectedAction);


		Ant::realizeAntsAction(ants, environment);

	} while (selectedAction != Ant::Action::DIE);//TODO Change this condition when there are multiple ants
	return 0;
}