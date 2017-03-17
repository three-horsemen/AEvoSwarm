//
// Created by reuben on 16/3/17.
//

#include <ant_world/AntWorld.hpp>

int main(int argc, char *argv[]) {
	AntWorld antWorld(50, 50);
	antWorld.setDisplayPeriod(antWorld.getCheckpointPeriod());
	antWorld.setWaitPeriod(150);
	antWorld.setMutationEnabled(false);
	antWorld.setCheckpointEnabled(false);
	antWorld.skipIterations(antWorld.getCheckpointPeriod());

//	antWorld.load(700);

	while (antWorld.isRunning()) {
		if (!antWorld.load(antWorld.getIteration()))
			return 0;
		antWorld.displayPeriodically();
		antWorld.skipIterations(antWorld.getCheckpointPeriod());
		antWorld.waitRemainingPeriod();
	}
	return 0;
}