//
// Created by reuben on 16/3/17.
//

#include <mpi.h>

#include <ant_world/AntWorld.hpp>

#define comm MPI_COMM_WORLD

using namespace std;

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	AntWorld antWorld(50, 50);
	antWorld.setDisplayPeriod(1);
	antWorld.setCheckpointPeriod(5000);
	antWorld.setWaitPeriod(1);
	antWorld.setMutationEnabled(true);
	antWorld.setCheckpointLocation(string("./checkpoints") + to_string(rank));

	antWorld.loadFromFile(2270000);

	while (antWorld.isRunning()) {

		antWorld.maintainMinimumPopulation();
		antWorld.performIteration();
		antWorld.displayPeriodically();
		antWorld.checkpointPeriodically();
		antWorld.waitOnePeriod();
	}

	MPI_Finalize();
	return 0;
}