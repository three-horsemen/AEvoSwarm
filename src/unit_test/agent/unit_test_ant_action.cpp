//
// Created by reuben on 5/3/17.
//

#include <ant_world/AntWorld.hpp>

int main(int argc, char *argv[]) {
	AntWorld antWorld(15, 15);
	antWorld.setMinimumPopulation(15);
	antWorld.setDisplayPeriod(1);
	antWorld.setCheckpointPeriod(10000);
	antWorld.setWaitPeriod(5000);
	antWorld.setMutationEnabled(true);
	antWorld.setMutationPeriod(100);
	antWorld.setCrossoverEnabled(false);
	antWorld.setCheckpointLocation("./checkpoints");

	try {
//		antWorld.loadFromFile(20000);
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

