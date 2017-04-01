//
// Created by reuben on 16/3/17.
//

#include <ant_world/AntWorld.hpp>

int main(int argc, char *argv[]) {
	AntWorld antWorld(50, 50);
	antWorld.setMinimumPopulation(40);
	antWorld.setDisplayPeriod(1);
	antWorld.setCheckpointPeriod(10000);
	antWorld.setWaitPeriod(1);
	antWorld.setMutationEnabled(true);
	antWorld.setMutationPeriod(100);
	antWorld.setCrossoverEnabled(false);
	antWorld.setCheckpointLocation("./checkpoints");

	try {
//		antWorld.loadFromFile(70000);
	} catch (runtime_error &e) {
		cerr << e.what() << endl;
		return 0;
	}

	while (antWorld.isRunning()) {
		try {
			antWorld.maintainMinimumPopulation();
		} catch (Exception &e) {
			cerr << e.what() << endl;
		}
		antWorld.performIteration();
		antWorld.displayPeriodically();
		antWorld.checkpointPeriodically();
		antWorld.waitOnePeriod();
	}
	return 0;
}