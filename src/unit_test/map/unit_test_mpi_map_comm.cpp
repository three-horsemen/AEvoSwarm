//
// Created by reuben on 8/3/17.
//

#include <mpi.h>
#include <shared/Utils.hpp>

#define comm MPI_COMM_WORLD
#define gW 10
#define gH 10
#define lW 3
#define lH 3

using namespace std;

int getGlobalIndex(int i, int j, int rank) {
	int x = rank % gW;
	int y = rank / gW;

	return Utils::modulo(y + i - 1, gH) * gW + Utils::modulo(x + j - 1, gW);
}

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_rank(comm, &size);


	int localMap[lH][lW];

	for (int i = 0; i < lW; i++) {
		for (int j = 0; j < lH; j++) {
			localMap[i][j] = (char) getGlobalIndex(i, j, rank);
		}
	}

	int recvMap[lH][lW][lH*lW];
	int s = 0;
	for (int i = 0; i < lW; i++) {
		for (int j = 0; j < lH; j++) {
			s += localMap[i][j];
			for (int p = 0; p < lW; p++) {
				for (int q = 0; q < lH; q++) {
					//recvMap[i][j][i*j]
					//MPI_Isend(&s,1,MPI_INT,)
				}
			}
			//MPI_Bcast(&recvMap[i][j], 1, MPI_INT, getGlobalIndex(i, j, rank), comm);
		}
	}
	localMap[1][1] = s;

	if (rank == 15) {
		for (int i = 0; i < lW; i++) {
			for (int j = 0; j < lH; j++) {
				cout << localMap[i][j] << '\t';
			}
			cout << endl;
		}
	}

	MPI_Finalize();
}