//
// Created by reuben on 16/3/17.
//

#include <ant_world/AntWorld.hpp>

int main(int argc, char *argv[]) {
	AntWorld antWorld(50, 50);
	antWorld.setDisplayPeriod(101);
	antWorld.setCheckpointPeriod(5000);
	antWorld.setWaitPeriod(1);
	antWorld.setMutationEnabled(true);
	antWorld.setCheckpointLocation("./checkpoints");

//	antWorld.load(700);

	while (antWorld.isRunning()) {
		antWorld.performIteration();
		antWorld.maintainMinimumPopulation();
		antWorld.displayPeriodically();
		antWorld.checkpointPeriodically();
		antWorld.waitOnePeriod();
	}
	return 0;
}