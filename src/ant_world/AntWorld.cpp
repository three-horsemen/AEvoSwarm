//
// Created by reuben on 16/3/17.
//

#include "ant_world/AntWorld.hpp"

AntWorld::AntWorld(int width, int height, bool geneticCrossoverEnabled) : environment((short) width, (short) height) {
	this->geneticCrossoverEnabled = geneticCrossoverEnabled;
	geneticCrossoverPeriod = 1000;
	if (geneticCrossoverEnabled) {
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		assert(size != 0);
	}

	srand((unsigned int) time(NULL) + rank);

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
	minimumPopulationEnabled = displayEnabled = fileCheckpointsEnabled = _isRunning = true;

	key = -1;
	waitPeriod = 250;
	selectedAnt = 0;
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
	if (geneticCrossoverEnabled && getIteration() % geneticCrossoverPeriod == 0) {
		return performGeneMixIteration((unsigned int) (5), (unsigned int) size);
	} else {
		return performRegularIteration();
	}
}

bool AntWorld::performRegularIteration() {
	Energy priorEnergy = environment.getTotalEnergy();

	switch (key) {
		case '[':
			if (selectedAnt > 0)
				selectedAnt--;
			cout << "Selected ant: " << selectedAnt << endl;
			break;
		case ']':
			if (selectedAnt < ants.size() - 1)
				selectedAnt++;
			cout << "Selected ant: " << selectedAnt << endl;
			break;
		default:
			cout << "Ignoring keypress '" << key << "'\n";
			break;
	}

	unsigned long long antCount = ants.size();
	for (unsigned long long j = 0; j < antCount; j++) {

		ants[j].observeEnvironment(environment);
		ants[j].senseObservation(environment);

		int key = this->key;
		if (selectedAnt == j) {
			Ant::Action selectedAction = (Ant::Action) -1;
			do {
				switch (key) {
					case 'w':
						selectedAction = Ant::FORWARD;
						break;
					case 97:
						selectedAction = Ant::LEFT;
						break;
					case 'd':
						selectedAction = Ant::RIGHT;
						break;
					case 'e':
						selectedAction = Ant::EAT;
						break;
					case 'q':
						selectedAction = Ant::ATTACK;
						break;
					case 'f':
						selectedAction = Ant::FORTIFY;
						break;
					case 'm':
						selectedAction = Ant::MATURE;
						break;
					case 'b':
						selectedAction = Ant::GROW_BABY;
						break;
					case 'n':
						selectedAction = Ant::GIVE_BIRTH;
						break;
					default:
						cout << "Ignoring keypress '" << key << "'\n";
						ants[j].selectAction();
						selectedAction = (Ant::Action) ants[j].getSelectedAction();
						break;
				}
				if (selectedAction != -1 && !ants[j].isActionValid((Agent::Action) selectedAction)) {
					selectedAction = (Ant::Action) -1;
				}
				if (selectedAction == -1) {
					key = waitKey(1);
				}
			} while (selectedAction == -1);
			assert(ants[j].isActionValid((Agent::Action) selectedAction));
			ants[j].setSelectedAction((Agent::Action) selectedAction);
		} else {
			ants[j].selectAction();
		}
		ants[j].performAction(ants[j].getSelectedAction());

		if (mutationEnabled && iteration % mutationPeriod == 0) {
			ants[j].mutate();
		}

	}

	Ant::realizeAntsAction(ants, environment);

	if (selectedAnt > ants.size() - 1)
		selectedAnt = ants.size() - 1;

	if (environment.getTotalEnergy() != priorEnergy) {
//		AsciiEnvironment::displayEnergyDeltas(oldEnvironment.getEnvironment(), environment);
		throw runtime_error(string("Environment energy is not conserved in iteration ") + to_string(iteration)
							+ " from " + to_string(priorEnergy) + " to " + to_string(environment.getTotalEnergy()));
	}

	iteration++;
	return true;
}

bool AntWorld::performGeneMixIteration(unsigned int sendCount, unsigned int size) {
	assert(ants.size() >= sendCount * size);
	Energy priorEnergy = environment.getTotalEnergy();

	stringbuf sendBuffer;
	ostream os(&sendBuffer);

	unsigned long long antCount = ants.size();

	for (unsigned long long j = 0; j < antCount; j++) {
		ants[j].observeEnvironment(environment);
		ants[j].senseObservation(environment);

		if (j < sendCount * size) {
			ants[j].saveWithCharacter(os);
			ants[j].setSelectedAction((Agent::Action) Ant::DIE);
		} else {
			if (mutationEnabled && iteration % mutationPeriod == 0) {
				ants[j].mutate();
			}
			ants[j].selectAction();
		}
		ants[j].performAction((Agent::Action) ((Ant::Action) ants[j].getSelectedAction()));
	}
	char *receiveBuf = new char[sendBuffer.str().size()];
	MPI_Alltoall(sendBuffer.str().c_str(), (int) (sendBuffer.str().size() / size), MPI_CHAR,
				 receiveBuf, (int) (sendBuffer.str().size() / size), MPI_CHAR, MPI_COMM_WORLD);

//	cout << "Received " << sendBuffer.str().size() << " bytes\n";

//	if (rand() % 2) {
//		cout<<"WRITING TO FILE\n";
//		ofstream ofs("./checkpoints/tmp.ants", ios::binary);
//		ofs.write(receiveBuf, sendBuffer.str().size());
//		ofs.close();
//	}

	membuf receivedBuf(receiveBuf, receiveBuf + sendBuffer.str().size());
	istream is(&receivedBuf);

//	ifstream is("./checkpoints/tmp.ants", ios::binary);

	Ant::realizeAntsAction(ants, environment);

	for (unsigned int i = 0; i < sendCount * size; i++) {
		Ant ant;
		ant.loadWithCharacter(is);
		Ant::sparkLifeWhereAvailable(environment, ants, ant);
	}

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
	if (fileCheckpointsEnabled && iteration % checkpointPeriod == 0) {
		saveToFile();
	}
}

void AntWorld::saveToFile() {
	string environmentFilePath = checkpointLocation + "/" + to_string(iteration) + ".env";
	cout << "Saving to file " << environmentFilePath << endl;
	ofstream environmentFile(environmentFilePath, ios::out | ios::binary);
	environment.save(environmentFile);
	environmentFile.close();

	string antFilePath = checkpointLocation + "/" + to_string(iteration) + ".ants";
	cout << "Saving to file " << antFilePath << endl;
	ofstream antFile(antFilePath, ios_base::out | ios_base::binary);
	Ant::save(antFile, ants);
	antFile.close();
}

void AntWorld::loadFromFile(unsigned long long iteration) {
	try {
		string environmentFilePath = checkpointLocation + "/" + to_string(iteration) + ".env";
		string antFilePath = checkpointLocation + "/" + to_string(iteration) + ".ants";
		cout << "Loading from file " << environmentFilePath << endl;
		cout << "Loading from file " << antFilePath << endl;

		ifstream antFile(antFilePath, ios_base::in | ios_base::binary);
		ifstream environmentFile(environmentFilePath, ios_base::in | ios_base::binary);
		if (!environmentFile.is_open())
			throw runtime_error("File could not be opened");
		if (!antFile.is_open())
			throw runtime_error("File could not be opened");

		if (!environment.load(environmentFile))
			throw runtime_error("Error loading the environment");
		if (!Ant::load(antFile, environment, ants))
			throw runtime_error("Error loading the ants");
		this->iteration = iteration;
	} catch (exception &e) {
		cout << e.what() << endl;
		throw e;
	}
}

void AntWorld::waitOnePeriod() {
	key = waitKey((int) waitPeriod);
	if (char(27) == key) {
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
	key = waitKey((int) waitPeriod);
	if (char(27) == key) {
		_isRunning = false;
	}
	previousWaitStartTimestamp = waitStartTimestamp;
}
