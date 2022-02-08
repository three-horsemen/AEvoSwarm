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

	OpenCLFullyConnectedLayer::staticInit();    //TODO Make this conditional on enabling OpenCL
}

AntWorld::~AntWorld() {
	delete openCVEnvironment;
	OpenCLFullyConnectedLayer::staticDeinit();    //TODO Make this conditional on enabling OpenCL
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
//			cout << "Ignoring keypress '" << key << "'\n";
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
//						cout << "Ignoring keypress '" << key << "'\n";
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

	realizeAntsAction();

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

	realizeAntsAction();

	for (unsigned int i = 0; i < sendCount * size; i++) {
		Ant ant;
		ant.loadWithCharacter(is);
		sparkLifeWhereAvailable(ant);
	}

	if (environment.getTotalEnergy() != priorEnergy) {
//		AsciiEnvironment::displayEnergyDeltas(oldEnvironment.getEnvironment(), environment);
		throw runtime_error(string("Environment energy is not conserved in iteration ") + to_string(iteration)
							+ " from " + to_string(priorEnergy) + " to " + to_string(environment.getTotalEnergy()));
	}

	iteration++;
	return true;
}

void AntWorld::realizeAntsAction() {
	realizeAntAttacks();
	unsigned long initialAntCount = ants.size(); //This may change if ants are born, or if they die.
	Environment environmentBackup(environment);
	environment.clearCharacterGrid();
	for (unsigned short i = 0; i < initialAntCount; i++) {
		affectEnvironment(i, environmentBackup);
	}
	haveAntsDieOfInjury();
	eraseDeadAnts();
}

void AntWorld::realizeAntAttacks() {
	vector<Coordinate> attackTargets;
	for (unsigned short i = 0; i < ants.size(); i++) {
		if ((Ant::Action) ants[i].getSelectedAction() == Ant::ATTACK) {
			attackTargets.push_back(ants[i].getGlobalCoordinate(environment, adjacency::AHEAD));
		}
	}
	for (unsigned short i = 0; i < attackTargets.size(); i++) {
		for (unsigned short j = 0; j < ants.size(); j++) {
			if (ants[j].getGlobalCoordinate() == attackTargets[i]
				&& ants[j].getShield() > 0)    //Simple optimization
				ants[j].beAttacked(ants[j].MAX_DAMAGE);
		}
	}
}

void AntWorld::affectEnvironment(unsigned short indexOfAnt, Environment &oldEnvironment) {
	assert(indexOfAnt < ants.size());
//	Energy priorEnergy = oldEnvironment.getTotalEnergy();

	//Special effect of GIVE_BIRTH
	if ((Ant::Action) ants[indexOfAnt].getSelectedAction() == Ant::GIVE_BIRTH) {
		Ant newborn;
		ants[indexOfAnt].pullOutNewborn(environment, newborn);
//		cout << "Newborn pulled out at (" << newborn.getGlobalCoordinate().getX() << ","
//			 << newborn.getGlobalCoordinate().getY() << ") with " << newborn.getTotalEnergy() << " energy\n";
		placeInEnvironment(newborn, newborn.getGlobalCoordinate());
		ants.push_back(newborn);
	}

	placeCharacterInEnvironment(ants[indexOfAnt], ants[indexOfAnt].getGlobalCoordinate());
	//Dead ants aren't placed in the environment, but may still have energy distribution effects in the environment.

	//Right now this is just updating the perceived energy changes.
	for (int x = 0; x < ants[indexOfAnt].getPerceptiveField()->width; x++) {
		for (int y = 0; y < ants[indexOfAnt].getPerceptiveField()->height; y++) {
			Tile perceptiveTile = ants[indexOfAnt].getPerceptiveField()->getTile(Coordinate(x, y));
			Tile globalTile = environment.getTile(perceptiveTile.getGlobalCoordinate());
			signed int differentialEnergyValue = perceptiveTile.getTotalEnergy() - oldEnvironment.getTile(
					perceptiveTile.getGlobalCoordinate()).getTotalEnergy();
			if (differentialEnergyValue) {
				globalTile.setTotalEnergy((Energy) (globalTile.getTotalEnergy() + differentialEnergyValue));
				environment.setTile(globalTile, globalTile.getGlobalCoordinate());
			}
		}
	}
}

void AntWorld::eraseDeadAnts() {
	for (int i = 0; i < ants.size(); i++)
		if ((ants[i].getCharacter().getOccupancy() == OCCUPANCY_DEAD) || (ants[i].getShield() <= 0)) {
			ants.erase(ants.begin() + i);
			i--;
		}
}

void AntWorld::haveAntsDieOfInjury() {
	for (unsigned short j = 0; j < ants.size(); j++) {
		if (!ants[j].getCharacter().getOccupancy() == OCCUPANCY_DEAD && ants[j].getShield() == 0) {
			ants[j].die();
		}
	}
}

void AntWorld::maintainMinimumPopulation() {
	if (minimumPopulationEnabled && ants.size() < minimumPopulation) {
		sparkNLives((unsigned int) (minimumPopulation - ants.size()));
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
	save(antFile);
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
		if (!load(antFile))
			throw runtime_error("Error loading the ants");
		this->iteration = iteration;
	} catch (exception &e) {
		cerr << e.what() << endl;
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

void AntWorld::placeInEnvironment(Ant &ant, Coordinate coordinate) {
	environment.setTile(
			(ant >> environment.getTile(coordinate)),
			coordinate
	);
}

void AntWorld::placeCharacterInEnvironment(Ant &ant, Coordinate coordinate) {
	environment.setTile(
			(ant >= environment.getTile(coordinate)),
			coordinate
	);
}

void AntWorld::sparkLifeAt(Ant &ant) {
	Tile tile = environment.getTile(ant.getGlobalCoordinate());
	if (tile.getAgentCharacter().getOccupancy() == OCCUPANCY_DEAD
		&& tile.getTotalEnergy() >= ant.getTotalEnergy()
		&& !Ant::isInImpactRange(environment, tile.getGlobalCoordinate())
			) {

		placeCharacterInEnvironment(ant, ant.getGlobalCoordinate());

		ants.push_back(ant);
//		cout << "Sparked ant at " << ant.getGlobalCoordinate().toString() << " with " << ant.getTotalEnergy() << " energy\n";
	} else {
		throw invalid_argument("Cannot spark life at " + ant.getGlobalCoordinate().toString());
	}
}

bool AntWorld::sparkLifeWhereAvailable(Ant &ant) {
	int randXOffset = rand() % environment.width;
	int randYOffset = rand() % environment.height;
	for (int x = 0; x < environment.width; x++) {
		for (int y = 0; y < environment.height; y++) {
			int X = (x + randXOffset) % environment.width;
			int Y = (y + randYOffset) % environment.height;

			Tile tile = environment.getTile(Coordinate(X, Y));
			if (tile.getAgentCharacter().getOccupancy() == OCCUPANCY_DEAD
				&& tile.getTotalEnergy() >= ant.getTotalEnergy()
				&& !Ant::isInImpactRange(environment, tile.getGlobalCoordinate())
					) {
				ant.setGlobalCoordinate(Coordinate(X, Y));
				AntWorld::sparkLifeAt(ant);
				return true;
			}
		}
	}
	return false;
}

void AntWorld::sparkNLives(unsigned int count) {
	Ant ant;

	int randXOffset = rand() % environment.width;
	int randYOffset = rand() % environment.height;
	for (int x = 0; x < environment.width && count > 0; x++) {
		for (int y = 0; y < environment.height && count > 0; y++) {
			int X = (x + randXOffset) % environment.width;
			int Y = (y + randYOffset) % environment.height;

			Tile tile = environment.getTile(Coordinate(X, Y));
			if (tile.getAgentCharacter().getOccupancy() == OCCUPANCY_DEAD
				&& tile.getTotalEnergy() >= Ant::NEWBORN_MIN_TOTAL_ENERGY
				&& !Ant::isInImpactRange(environment, tile.getGlobalCoordinate())
					) {
				ant.randomize();
				ant.setPotential(0);
				ant.setPushedFetalEnergy(0);
				ant.setFertility(Ant::NEWBORN_MIN_FERTILITY);
				ant.setFetal(0);
				ant.setShield(Ant::NEWBORN_MIN_SHIELD);
				ant.setPotential(tile.getTotalEnergy() - ant.getTotalEnergy());
				ant.setGlobalCoordinate(Coordinate(X, Y));
				AntWorld::sparkLifeAt(ant);
				count--;
			}
		}
	}
	if (count > 0) {
		throw runtime_error(std::string("Not enough energy to spark remaining ") + to_string(count) + " ants");
	}
}

void AntWorld::save(ostream &file) {
	file <= ants.size();
	for (unsigned long i = 0; i < ants.size(); i++) {
		ants[i].save(file);
	}
}

bool AntWorld::load(istream &file) {
	try {
		ants.clear();
		unsigned long int size;
		file >= size;
//		while(file>>size)	cout<<size<<endl;
		for (int i = 0; i < size; i++) {
			Ant ant;
			ant.load(file, environment);
			ants.push_back(ant);
		}
		return true;
	} catch (exception &e) {
		cerr << e.what();
		return false;
	}
}
