//
// Created by reuben on 16/3/17.
//

#ifndef AEVO_ANTWORLD_HPP
#define AEVO_ANTWORLD_HPP

#include <ui/OpenCV.hpp>
#include <ui/AsciiEnvironment.hpp>
#include <chrono>

#include <mpi.h>

struct membuf : std::streambuf {
	membuf(char *begin, char *end) {
		this->setg(begin, begin, end);
	}
};

class AntWorld {
	vector<Ant> ants;
	ui::OpenCV *openCVEnvironment;
	Environment environment;

	int rank, size;

	int key;
	unsigned long selectedAnt;

	string checkpointLocation;

	unsigned long long iteration;

	bool mutationEnabled;
	unsigned int mutationPeriod;

	bool minimumPopulationEnabled;
	unsigned int minimumPopulation;

	bool displayEnabled;
	unsigned int displayPeriod;

	bool fileCheckpointsEnabled;
	unsigned int checkpointPeriod;

	bool geneticCrossoverEnabled;
	unsigned int geneticCrossoverPeriod;

	std::chrono::milliseconds previousWaitStartTimestamp;
	unsigned long waitPeriod;
	long deficit;

	bool _isRunning;
public:
	AntWorld(int, int, bool geneticCrossoverEnabled = false);

	~AntWorld();

	bool isRunning();

	unsigned long long getIteration();

	void skipIterations(unsigned long long);

	bool performIteration();

	bool performRegularIteration();

	bool performGeneMixIteration(unsigned int, unsigned int);

	void maintainMinimumPopulation();

	void displayPeriodically();

	void checkpointPeriodically();

	void saveToFile();

	void loadFromFile(unsigned long long);

	void waitOnePeriod();

	void waitRemainingPeriod();

	inline void setMutationEnabled(bool enabled) {
		mutationEnabled = enabled;
	}

	inline void setMutationPeriod(unsigned int period) {
		mutationPeriod = period;
	}

	inline void setMinimumPopulationEnabled(bool enabled) {
		minimumPopulationEnabled = enabled;
	}

	inline bool getMinimumPopulationEnabled() {
		return minimumPopulationEnabled;
	}

	inline void setMinimumPopulation(unsigned int minPopulation) {
		minimumPopulation = minPopulation;
	}

	inline unsigned int getMinimumPopulation() {
		return minimumPopulation;
	}

	inline void setDisplayEnabled(bool enabled) {
		displayEnabled = enabled;
	}

	inline bool getDisplayEnabled() {
		return displayEnabled;
	}

	inline void setDisplayPeriod(unsigned int period) {
		displayPeriod = period;
	}

	inline unsigned int getDisplayPeriod() {
		return displayPeriod;
	}

	inline void setFileCheckpointsEnabled(bool enabled) {
		fileCheckpointsEnabled = enabled;
	}

	inline bool getFileCheckpointsEnabled() {
		return fileCheckpointsEnabled;
	}

	inline void setCheckpointPeriod(unsigned int period) {
		checkpointPeriod = period;
	}

	inline unsigned int getCheckpointPeriod() {
		return checkpointPeriod;
	}

	inline void setWaitPeriod(unsigned long period) {
		waitPeriod = period;
	}

	inline unsigned long getWaitPeriod() {
		return waitPeriod;
	}

	inline void setCheckpointLocation(string path) {
		checkpointLocation = path;
	}

	inline string getCheckpointLocation() {
		return checkpointLocation;
	}

	inline void setCrossoverEnabled(bool enabled) {
		geneticCrossoverEnabled = enabled;
		if (geneticCrossoverEnabled) {
			MPI_Comm_rank(MPI_COMM_WORLD, &rank);
			MPI_Comm_size(MPI_COMM_WORLD, &size);
		}
	}

	inline void setCrossoverPeriod(unsigned int period) {
		geneticCrossoverPeriod = period;
	}

	inline vector<Ant> &getAnts() {
		return ants;
	}

	inline Environment &getEnvironment() {
		return environment;
	}

	inline Environment &getOldEnvironment() {
		return environment;
	}
};


#endif //AEVO_ANTWORLD_HPP
