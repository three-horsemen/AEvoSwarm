//
// Created by reuben on 16/3/17.
//

#include "ant_world/AntWorld.hpp"

AntWorld::AntWorld(int width, int height) : environment((short) width, (short) height) {
	srand((unsigned int) time(NULL));
	environment = Environment((short) width, (short) height);
	openCVEnvironment = new ui::OpenCV(this->environment);

	checkpointLocation = ".";
	iteration = 0;
	mutationPeriod = 500;
	minimumPopulation = (unsigned int) width;
	displayPeriod = 1;
	checkpointPeriod = 5000;

	environment.randomize();

	mutationEnabled = false;
	minimumPopulationEnabled = displayEnabled = checkpointEnabled = _isRunning = true;

	waitPeriod = 250;
	deficit = 0;
	previousWaitStartTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
	);
}

AntWorld::~AntWorld() {
	delete openCVEnvironment;
}

bool AntWorld::isRunning() {
	return _isRunning;
}

unsigned long long AntWorld::getIteration() {
	return iteration;
}

void AntWorld::skipIterations(unsigned long long skip) {
	iteration += skip;
}

bool AntWorld::performIteration() {
	Energy priorEnergy = environment.getTotalEnergy();

	unsigned long long antCount = ants.size();
	for (unsigned long long j = 0; j < antCount; j++) {

		ants[j].observeEnvironment(environment);
		ants[j].senseObservation(environment);
		ants[j].selectAction();
		ants[j].performAction((Agent::Action) ((Ant::Action) ants[j].getSelectedAction()));

		if (mutationEnabled && iteration % mutationPeriod == 0) {
			ants[j].mutate();
		}

	}

	Ant::realizeAntsAction(ants, environment);

	if (environment.getTotalEnergy() != priorEnergy) {
//		AsciiEnvironment::displayEnergyDeltas(oldEnvironment.getEnvironment(), environment);
		throw runtime_error(string("Environment energy is not conserved in iteration ") + to_string(iteration)
							+ " from " + to_string(priorEnergy) + " to " + to_string(environment.getTotalEnergy()));
	}

	iteration++;
	return true;
}

void AntWorld::maintainMinimumPopulation() {
	if (minimumPopulationEnabled && ants.size() < minimumPopulation) {
		Ant::sparkNLives(environment, ants, (unsigned int) (minimumPopulation - ants.size()));
	}
}

void AntWorld::displayPeriodically() {
	if (displayEnabled && iteration % displayPeriod == 0) {
		openCVEnvironment->displayEnvironment(ants, iteration);
	}
}

void AntWorld::checkpointPeriodically() {
	if (checkpointEnabled && iteration % checkpointPeriod == 0) {
		environment.save(checkpointLocation + "/" + "environment" + to_string(iteration) + ".txt");
		Ant::save(checkpointLocation + "/" + "ants" + to_string(iteration) + ".txt", ants);
	}
}

void AntWorld::waitOnePeriod() {
	if (char(27) == waitKey((int) waitPeriod)) {
		_isRunning = false;
	}
	previousWaitStartTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
	);
}

void AntWorld::waitRemainingPeriod() {
	std::chrono::milliseconds waitStartTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
	);
	std::chrono::milliseconds elapsed = waitStartTimestamp - previousWaitStartTimestamp;
	deficit += waitPeriod - elapsed.count();

	long remainingTime = std::min(waitPeriod, (unsigned long) std::max(deficit, (long) 1));
	cout << deficit << " deficit" << " and waiting " << remainingTime << endl;
	if (char(27) == waitKey((int) remainingTime)) {
		_isRunning = false;
	}
	previousWaitStartTimestamp = waitStartTimestamp;
}

bool AntWorld::load(unsigned long long iteration) {
	if (!environment.load(checkpointLocation + "/" + "environment" + to_string(iteration) + ".txt"))
		return false;
	if (!Ant::load(checkpointLocation + "/" + "ants" + to_string(iteration) + ".txt", environment, ants))
		return false;
	this->iteration = iteration;
	return true;
}

void AntWorld::setMutationEnabled(bool enabled) {
	mutationEnabled = enabled;
}

void AntWorld::setMutationPeriod(unsigned int period) {
	mutationPeriod = period;
}

void AntWorld::setMinimumPopulationEnabled(bool enabled) {
	minimumPopulationEnabled = enabled;
}

bool AntWorld::getMinimumPopulationEnabled() {
	return minimumPopulationEnabled;
}

void AntWorld::setMinimumPopulation(unsigned int minPopulation) {
	minimumPopulation = minPopulation;
}

unsigned int AntWorld::getMinimumPopulation() {
	return minimumPopulation;
}

void AntWorld::setDisplayEnabled(bool enabled) {
	displayEnabled = enabled;
}

bool AntWorld::getDisplayEnabled() {
	return displayEnabled;
}

void AntWorld::setDisplayPeriod(unsigned int period) {
	displayPeriod = period;
}

unsigned int AntWorld::getDisplayPeriod() {
	return displayPeriod;
}

void AntWorld::setCheckpointEnabled(bool enabled) {
	checkpointEnabled = enabled;
}

bool AntWorld::getCheckpointEnabled() {
	return checkpointEnabled;
}

void AntWorld::setCheckpointPeriod(unsigned int period) {
	checkpointPeriod = period;
}

unsigned int AntWorld::getCheckpointPeriod() {
	return checkpointPeriod;
}

void AntWorld::setWaitPeriod(unsigned long period) {
	waitPeriod = period;
}

unsigned long AntWorld::getWaitPeriod() {
	return waitPeriod;
}

void AntWorld::setCheckpointLocation(string path) {
	checkpointLocation = path;
}

string AntWorld::getCheckpointLocation() {
	return checkpointLocation;
}