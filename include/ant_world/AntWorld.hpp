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

	bool loadFromFile(unsigned long long);

	void waitOnePeriod();

	void waitRemainingPeriod();

	void setMutationEnabled(bool);

	bool getMutationEnabled();

	void setMutationPeriod(unsigned int);

	unsigned int getMutationPeriod();

	void setMinimumPopulationEnabled(bool);

	bool getMinimumPopulationEnabled();

	void setMinimumPopulation(unsigned int);

	unsigned int getMinimumPopulation();

	void setDisplayEnabled(bool);

	bool getDisplayEnabled();

	void setDisplayPeriod(unsigned int);

	unsigned int getDisplayPeriod();

	void setFileCheckpointsEnabled(bool);

	bool getFileCheckpointsEnabled();

	void setCheckpointPeriod(unsigned int);

	unsigned int getCheckpointPeriod();

	void setWaitPeriod(unsigned long);

	unsigned long getWaitPeriod();

	void setCheckpointLocation(string);

	string getCheckpointLocation();

	void setCrossoverEnabled(bool);

	void setCrossoverPeriod(unsigned int);

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
