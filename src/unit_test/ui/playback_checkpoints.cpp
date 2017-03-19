//
// Created by reuben on 16/3/17.
//

#include <ant_world/AntWorld.hpp>

int main(int argc, char *argv[]) {
	AntWorld antWorld(50, 50);
	antWorld.setWaitPeriod(500);
	antWorld.setMutationEnabled(false);
	antWorld.setCheckpointLocation("./checkpoints0");
	antWorld.setCheckpointPeriod(10000);
	antWorld.setDisplayPeriod(antWorld.getCheckpointPeriod());
	antWorld.setFileCheckpointsEnabled(false);

	antWorld.skipIterations(90000);

//	antWorld.loadFromFile(10000);

	try {
		while (antWorld.isRunning()) {
			antWorld.loadFromFile(antWorld.getIteration());
			antWorld.displayPeriodically();
			antWorld.skipIterations(antWorld.getCheckpointPeriod());
			antWorld.waitRemainingPeriod();
		}
	} catch (exception &e) {
		cout << e.what() << endl;
	}
	return 0;
}