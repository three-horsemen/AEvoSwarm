//
// Created by reuben on 16/3/17.
//

#include <mpi.h>

#include <ant_world/AntWorld.hpp>

#define comm MPI_COMM_WORLD

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);

	cout << "Initializing rank " << rank << endl;

	AntWorld antWorld(50, 50, (unsigned int) time(NULL) + rank);
	antWorld.setDisplayPeriod(101);
	antWorld.setCheckpointPeriod(5000);
	antWorld.setWaitPeriod(1);
	antWorld.setMutationEnabled(true);
	antWorld.setCheckpointLocation(string("./checkpoints") + to_string(rank));

//	antWorld.load(700);

	while (antWorld.isRunning()) {
		antWorld.performIteration();
		antWorld.maintainMinimumPopulation();
		antWorld.displayPeriodically();
		antWorld.checkpointPeriodically();
		antWorld.waitOnePeriod();

		if (antWorld.getIteration() % 500 == 0) {

		}
	}

	MPI_Finalize();
	return 0;
}