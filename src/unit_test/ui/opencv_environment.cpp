//
// Created by soorya on 10/3/17.
//

#include <ui/OpenCV.hpp>
#include <ui/AsciiEnvironment.hpp>

int main(int argc, char *argv[]) {
	srand((unsigned int) time(NULL));

	Coordinate randomCoordinate(0, 0);

	vector<Ant> ants(1);

	ants[0].randomize();
	AgentCharacter agentCharacter = ants[0].getCharacter();
	agentCharacter.setOccupancy(OCCUPANCY_NORTH);
	ants[0].setCharacter(agentCharacter);
	ants[0].setPotential(60000);
	Environment environment(50, 50);
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

	for (unsigned long long i = 0; i < 10000000 && ants.size() > 0 && (pressedKey != escapeKey); i++) {

		AsciiEnvironment oldEnvironment(environment);
//		AsciiEnvironment::displayEnergyMatrix(environment);
		Energy priorEnergy = environment.getTotalEnergy();
//		cout << "Total energy present on the random environment: " << environment.getTotalEnergy() << endl;
		unsigned long long antCount = ants.size();
		for (unsigned long long j = 0; j < antCount; j++) {
//			ants[j].setSelectedAction((Agent::Action)Ant::Action::FORTIFY);

			ants[j].observeEnvironment(environment);
			ants[j].senseObservation(environment);
			ants[j].selectAction();
//			cout << "Ant " << j << " selected action " << ants[j].getSelectedAction() << endl;
			ants[j].performAction((Agent::Action) ((Ant::Action) ants[j].getSelectedAction()));

//			AsciiEnvironment::displayEnergyMatrix(*ants[j].getPerceptiveField());
			if (i % 51 == 0) {
//				ants[j].mutate();
			}

		}

		Ant::realizeAntsAction(ants, environment);

		if (ants.size() < 50) {
			Ant::sparkNLives(environment, ants, (unsigned int) (50 - ants.size()));
		}
		if (i % 101 == 0) {
			openCVEnvironment.displayEnvironment(ants, i);
			pressedKey = (char) waitKey(1);
//				cout << "ants[" << j << "] selected action: " << ants[j].getSelectedAction() << " with potential: "<< ants[j].getPotential() << endl;
		}
		if (i % 100 == 0) {
			environment.save("environment" + to_string(i) + ".txt");
			Ant::save("ants" + to_string(i) + ".txt", ants);
		}
		if (environment.getTotalEnergy() != priorEnergy) {
			AsciiEnvironment::displayEnergyDeltas(oldEnvironment.getEnvironment(), environment);
			cout << "Environment energy is not conserved from " << priorEnergy << " to " << environment.getTotalEnergy()
				 << endl;
		}
//		AsciiEnvironment::displayEnergyDeltas(oldEnvironment.getEnvironment(), environment);
//		cout << "Environment energy transitioned from " << priorEnergy << " to " << environment.getTotalEnergy()
//			 << endl;
	}
	return 0;
}