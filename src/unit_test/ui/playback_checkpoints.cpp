//
// Created by reuben on 16/3/17.
//

#include <ant_world/AntWorld.hpp>

int main(int argc, char *argv[]) {
	AntWorld antWorld(50, 50);
	antWorld.setWaitPeriod(1000);
	antWorld.setMutationEnabled(false);
	antWorld.setCheckpointLocation("./checkpoints");
	antWorld.setCheckpointPeriod(5000);
	antWorld.setDisplayPeriod(antWorld.getCheckpointPeriod());
	antWorld.setFileCheckpointsEnabled(false);
	antWorld.skipIterations(antWorld.getCheckpointPeriod());

//	antWorld.loadFromFile(10000);

	while (antWorld.isRunning()) {
		if (!antWorld.loadFromFile(antWorld.getIteration()))
			return 0;
		antWorld.displayPeriodically();
		antWorld.skipIterations(antWorld.getCheckpointPeriod());
		antWorld.waitRemainingPeriod();
	}
	return 0;
}