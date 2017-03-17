//
// Created by reuben on 16/3/17.
//

#ifndef AEVO_ANTWORLD_HPP
#define AEVO_ANTWORLD_HPP

#include <ui/OpenCV.hpp>
#include <ui/AsciiEnvironment.hpp>
#include <chrono>

class AntWorld {
	vector<Ant> ants;
	ui::OpenCV *openCVEnvironment;
	Environment environment;

	string checkpointLocation;

	unsigned long long iteration;

	bool mutationEnabled;
	unsigned int mutationPeriod;

	bool minimumPopulationEnabled;
	unsigned int minimumPopulation;

	bool displayEnabled;
	unsigned int displayPeriod;

	bool checkpointEnabled;
	unsigned int checkpointPeriod;

	std::chrono::milliseconds previousWaitStartTimestamp;
	unsigned long waitPeriod;
	long deficit;

	bool _isRunning;
public:
	AntWorld(int, int);

	~AntWorld();

	bool isRunning();

	unsigned long long getIteration();

	void skipIterations(unsigned long long);

	bool performIteration();

	void maintainMinimumPopulation();

	void displayPeriodically();

	void checkpointPeriodically();

	void waitOnePeriod();

	void waitRemainingPeriod();

	bool load(unsigned long long);

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

	void setCheckpointEnabled(bool);

	bool getCheckpointEnabled();

	void setCheckpointPeriod(unsigned int);

	unsigned int getCheckpointPeriod();

	void setWaitPeriod(unsigned long);

	unsigned long getWaitPeriod();

	void setCheckpointLocation(string);

	string getCheckpointLocation();
};


#endif //AEVO_ANTWORLD_HPP
