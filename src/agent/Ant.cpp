//
// Created by reuben on 5/3/17.
//

#include <agent/Ant.hpp>

short Ant::actionCost[actionCount] = {8, 6, 6, 2, 20, 15, 25, 15, 25, 0};

Ant::Ant() : Agent(5, 5) {
	developBrain();
}

Ant::Ant(Coordinate newCoordinate, Energy newPotential, Energy newShield, Energy newFertility, Energy newBaby,
		 AgentCharacter newCharacter) : Agent(5, 5) {
	coordinate = newCoordinate;
	potential = newPotential;
	shield = newShield;
	fertility = newFertility;
	baby = newBaby;
	character = newCharacter;
	if (newCharacter.getOccupancy() == OCCUPANCY_DEAD)
		throw invalid_argument("An ant can't be born dead!");

	developBrain();
}

Ant::~Ant() {
	resorbBrain();
}

Coordinate Ant::getCoordinate(Coordinate coordinate, Occupancy occupancy, adjacency::Adjacency adjacency) {
	if (adjacency == adjacency::UNDER)
		return coordinate;
	int x = coordinate.getX(), y = coordinate.getY();
	switch (occupancy) {
		case OCCUPANCY_NORTH:
			if (adjacency == adjacency::AHEAD)
				y--;
			else if (adjacency == adjacency::BEHIND)
				y++;
			else if (adjacency == adjacency::LEFT)
				x--;
			else
				x++;
			break;
		case OCCUPANCY_SOUTH:
			if (adjacency == adjacency::AHEAD)
				y++;
			else if (adjacency == adjacency::BEHIND)
				y--;
			else if (adjacency == adjacency::LEFT)
				x++;
			else
				x--;
			break;
		case OCCUPANCY_EAST:
			if (adjacency == adjacency::AHEAD)
				x++;
			else if (adjacency == adjacency::BEHIND)
				x--;
			else if (adjacency == adjacency::LEFT)
				y--;
			else
				y++;
			break;
		case OCCUPANCY_WEST:
			if (adjacency == adjacency::AHEAD)
				x--;
			else if (adjacency == adjacency::BEHIND)
				x++;
			else if (adjacency == adjacency::LEFT)
				y++;
			else
				y--;
			break;
		default:
			throw invalid_argument("Unknown occupancy specified");
	}
	return Coordinate(x, y);
}

Coordinate Ant::getLocalCoordinate(Occupancy occupancy, adjacency::Adjacency adjacency) {
	return getCoordinate(Coordinate(perceptiveField.width / 2, perceptiveField.height / 2), occupancy, adjacency);
}

Coordinate Ant::getLocalCoordinate(adjacency::Adjacency adjacency) {
	return getCoordinate(Coordinate(perceptiveField.width / 2, perceptiveField.height / 2), character.getOccupancy(),
						 adjacency);
}

Coordinate Ant::getLocalCoordinate() {
	return getCoordinate(Coordinate(perceptiveField.width / 2, perceptiveField.height / 2), character.getOccupancy(),
						 adjacency::UNDER);
}

bool Ant::isEnergyAvailable(Agent::Action action) {
	return getPotential() > actionCost[action];
}

bool Ant::isActionValid(Agent::Action agentAction) {
	if (!isEnergyAvailable(agentAction)) return false;

	Ant::Action action = (Action) agentAction;

	Coordinate coordinateAhead;
	switch (action) {
		case Ant::FORWARD:
			if (perceptiveField.getTile(
					getLocalCoordinate(adjacency::AHEAD)).getAgentCharacter().getOccupancy() !=
				OCCUPANCY_DEAD)
				return false;
			coordinateAhead = getLocalCoordinate(adjacency::AHEAD);
			for (int direction = OCCUPANCY_NORTH;
				 direction <= ((int) OCCUPANCY_WEST); direction++) {
				Coordinate potentialPredatorCoordinate = getCoordinate(coordinateAhead, (Occupancy) direction,
																	   adjacency::AHEAD);
				Tile potentialPredatorTile = perceptiveField.getTile(potentialPredatorCoordinate);

				if (potentialPredatorCoordinate != this->coordinate
					&& potentialPredatorTile.getAgentCharacter().getOccupancy() != OCCUPANCY_DEAD) {
					Coordinate rangeOfImpactCoordinate = getCoordinate(potentialPredatorCoordinate,
																	   potentialPredatorTile
																			   .getAgentCharacter().getOccupancy(),
																	   adjacency::AHEAD);
					if (rangeOfImpactCoordinate == coordinateAhead &&
						potentialPredatorTile.getTotalEnergy() >= this->getTotalEnergy()) {
						return false;
					}
				}
			}
			return true;
		case Ant::LEFT:
		case Ant::RIGHT:
			return true;
		case Ant::EAT:
			return perceptiveField.getTile(
					getLocalCoordinate()).getTotalEnergy() >
				   getTotalEnergy();
		case Ant::ATTACK:
			return perceptiveField.getTile(
					getLocalCoordinate(adjacency::AHEAD)).getAgentCharacter().getOccupancy();
		case Ant::FORTIFY:
		case Ant::MATURE:
		case Ant::GROW_BABY:
			return true;
		case Ant::GIVE_BIRTH:
			return !perceptiveField.getTile(
					getLocalCoordinate(adjacency::BEHIND)).getAgentCharacter().getOccupancy();
		case Ant::DIE:
			return true;
		default:
			throw invalid_argument("Unknown action specified");
	}
}

void Ant::observeEnvironment(Environment &environment) {
	for (int i = -perceptiveField.height / 2;
		 i < perceptiveField.height / 2; i++) {
		for (int j = -perceptiveField.width / 2;
			 j < +perceptiveField.width / 2; j++) {
			int x = Utils::modulo(coordinate.getX() + j, environment.width), y = Utils::modulo(coordinate.getY() + i,
																							   environment.width);
			Tile tile = environment.getTile(Coordinate(x, y));
			perceptiveField.setTile(tile, Coordinate(j + perceptiveField.width / 2, i + perceptiveField.height / 2));
		}
	}
}

void Ant::senseObservation(Environment &environment) {
	//This function will return a vector of three average senses from three vectors.
	//Taking for granted: the excitation pointer has been allocated memory.
	Attitude maxAttitude = 0;
	do {
		maxAttitude--;
	} while (maxAttitude < 0);
	Trait maxTrait = 0;
	do {
		maxTrait--;
	} while (maxTrait < 0);

	int ptr = 0;
	for (int i = 0; i < ant::sensor::SENSOR_COUNT; i++) {
		for (int j = 0; j < ant::percept::PERCEPT_COUNT; j++) {
			sensoryInputs[ptr++] = getSensation((sensor::Sensor) i, (percept::Percept) j);
		}
	}
	sensoryInputs[ptr++] = getPotential() / getMaxPerceptValue(percept::ENERGY);
	sensoryInputs[ptr++] = getShield() / getMaxPerceptValue(percept::ENERGY);
	sensoryInputs[ptr++] = getFertility() / getMaxPerceptValue(percept::ENERGY);
	sensoryInputs[ptr++] = getBaby() / getMaxPerceptValue(percept::ENERGY);
	sensoryInputs[ptr++] = getCharacter().getAttitude() / maxAttitude;
	sensoryInputs[ptr] = getCharacter().getTrait() / maxTrait;
}

void Ant::selectAction() {
	//neuron::randomizeExcitation(brain.getLayer(0)->inputSize, sensoryInputs);
	brain.getLayer(0)->setInputs(sensoryInputs);
	brain.compute();
	excitation *outputs = brain.getOutputLayer()->getOutputs();
	for (int i = 0; i < memoryCount; i++) {
		sensoryInputs[senseCount + i] = outputs[actionCount + i];
	}

	assert(brain.getOutputLayer()->outputSize == actionCount + memoryCount);

	outputs[DIE] = 0;
	int mostExcitedValidAction = -1;
	float maxExcitation = -1;
	for (int action = 0; action < actionCount; action++) {
		if (isActionValid((Agent::Action) action) && outputs[action] > maxExcitation) {
			mostExcitedValidAction = action;
			maxExcitation = outputs[action];
		}
	}

	assert(mostExcitedValidAction != -1);
	selectedAction = (Agent::Action) mostExcitedValidAction;
}

Agent::Action Ant::getSelectedAction() {
	return selectedAction;
}

void Ant::performAction(Agent::Action agentAction) {
	if (!isActionValid(agentAction)) throw invalid_argument("The provided action is invalid");
	Action action = (Action) agentAction;

	Energy potentialEnergy = getPotential();
	potentialEnergy -= actionCost[agentAction];
	setPotential(potentialEnergy);

	switch (action) {
		case Ant::FORWARD:
			moveForward();
			break;
		case Ant::LEFT:
			turnLeft();
			break;
		case Ant::RIGHT:
			turnRight();
			break;
		case Ant::EAT:
			eat();
			break;
		case Ant::ATTACK:
			attack();
			break;
		case Ant::FORTIFY:
			fortify();
			break;
		default:
			//TODO Complete remaining actions
			throw invalid_argument("Undefined action selected to be performed");
	}
}

void Ant::affectEnvironment(Environment &environment) {
	//TODO Blindly set character of agent onto map
	//TODO Additively incorporate perceptive field energy changes onto environment
}

void Ant::realizeAntsAction(vector<Ant> &ants, Environment &environment) {
	for (int i = 0; i < ants.size(); i++) {
		ants[i].affectEnvironment(environment);
	}
}

void Ant::developBrain() {
	const short inputSize = senseCount + memoryCount, fC1Size = (senseCount + memoryCount) * 9 / 10, fC2Size =
			(senseCount + memoryCount) * 81 / 100, outputSize =
			actionCount + memoryCount;
	InputLayer *inputLayer = new InputLayer(inputSize);
	excitation excitations[inputSize];
	neuron::randomizeExcitation(inputSize, excitations);
	inputLayer->setInputs((excitation *) excitations);
	brain.addLayer((Layer &) *inputLayer);

	weight weights1[fC1Size][inputSize];
	neuron::randomizeWeights(inputSize, fC1Size, (weight *) weights1);
	FullyConnectedLayer *fullyConnectedLayer1 = new FullyConnectedLayer(inputSize, fC1Size, (weight *) weights1);
	brain.addLayer((Layer &) *fullyConnectedLayer1);

	weight weights2[fC2Size][fC1Size];
	neuron::randomizeWeights(fC1Size, fC2Size, (weight *) weights2);
	FullyConnectedLayer *fullyConnectedLayer2 = new FullyConnectedLayer(fC1Size, fC2Size, (weight *) weights2);
	brain.addLayer((Layer &) *fullyConnectedLayer2);

	weight weights3[outputSize][fC2Size];
	neuron::randomizeWeights(fC2Size, outputSize, (weight *) weights3);
	FullyConnectedLayer *fullyConnectedLayer3 = new FullyConnectedLayer(fC2Size, outputSize, (weight *) weights3);
	brain.addLayer((Layer &) *fullyConnectedLayer3);

	OutputLayer *outputLayer = new OutputLayer(outputSize);
	brain.addLayer((Layer &) *outputLayer);

	sensoryInputs = new excitation[inputSize];
	neuron::randomizeExcitation(inputSize, excitations);
	inputLayer->setInputs((excitation *) excitations);
	brain.compute();
}

void Ant::resorbBrain() {
	delete[] sensoryInputs;
	brain.freeLayers();
}

Coordinate Ant::getGlobalCoordinate(Occupancy occupancy, adjacency::Adjacency adjacency) {
	return getCoordinate(coordinate, occupancy, adjacency);
}

Coordinate Ant::getGlobalCoordinate(adjacency::Adjacency adjacency) {
	return getCoordinate(coordinate, character.getOccupancy(), adjacency);
}

Coordinate Ant::getGlobalCoordinate() {
	return coordinate;
}

Energy Ant::getPotential() {
	return potential;
}

Energy Ant::getShield() {
	return shield;
}

Energy Ant::getFertility() {
	return fertility;
}

Energy Ant::getBaby() {
	return baby;
}

AgentCharacter Ant::getCharacter() {
	return character;
}

void Ant::setCoordinate(Coordinate newCoordinate) {
	coordinate = newCoordinate;
}

void Ant::setPotential(Energy newPotential) {
	potential = newPotential;
}

void Ant::setShield(Energy newShield) {
	shield = newShield;
}

void Ant::setFertility(Energy newFertility) {
	fertility = newFertility;
}

void Ant::setBaby(Energy newBaby) {
	baby = newBaby;
}

void Ant::setCharacter(AgentCharacter newCharacter) {
	character.setAttitude(newCharacter.getAttitude());
	character.setOccupancy(newCharacter.getOccupancy());
	character.setTrait(newCharacter.getTrait());
}

Energy Ant::getTotalEnergy() {
	return potential + shield + fertility + baby;
}

Tile Ant::operator<<(Tile tile) {
	tile.setAgentCharacter(AgentCharacter());
	tile.setTotalEnergy(tile.getTotalEnergy() - this->getTotalEnergy());
	return tile;
}

Tile Ant::operator>>(Tile tile) {
	tile.setAgentCharacter(character);
	tile.setTotalEnergy(tile.getTotalEnergy() + this->getTotalEnergy());
	coordinate = tile.getGlobalCoordinate();
	return tile;
}

Tile Ant::operator>(Tile tile) {
	Occupancy currentOccupancy = character.getOccupancy();
	Occupancy newOccupancy = OCCUPANCY_DEAD;
	if (currentOccupancy == OCCUPANCY_NORTH)
		newOccupancy = OCCUPANCY_EAST;
	else if (currentOccupancy == OCCUPANCY_EAST)
		newOccupancy = OCCUPANCY_SOUTH;
	else if (currentOccupancy == OCCUPANCY_SOUTH)
		newOccupancy = OCCUPANCY_WEST;
	else if (currentOccupancy == OCCUPANCY_WEST)
		newOccupancy = OCCUPANCY_NORTH;
	character.setOccupancy(newOccupancy);

	tile.setAgentCharacter(character);
	tile.setTotalEnergy(this->getTotalEnergy());
	return tile;
}

Tile Ant::operator<(Tile tile) {
	Occupancy currentOccupancy = character.getOccupancy();
	Occupancy newOccupancy = OCCUPANCY_DEAD;
	if (currentOccupancy == OCCUPANCY_NORTH)
		newOccupancy = OCCUPANCY_WEST;
	else if (currentOccupancy == OCCUPANCY_EAST)
		newOccupancy = OCCUPANCY_NORTH;
	else if (currentOccupancy == OCCUPANCY_SOUTH)
		newOccupancy = OCCUPANCY_EAST;
	else if (currentOccupancy == OCCUPANCY_WEST)
		newOccupancy = OCCUPANCY_SOUTH;
	character.setOccupancy(newOccupancy);

	tile.setAgentCharacter(character);
	tile.setTotalEnergy(this->getTotalEnergy());
	return tile;
}

void Ant::moveForward() {
	Coordinate perceivedSourceCoordinate(perceptiveField.width / 2, perceptiveField.height / 2);
	Coordinate perceivedDestinationCoordinate = getCoordinate(perceivedSourceCoordinate,
															  getCharacter().getOccupancy(), adjacency::AHEAD);

	perceptiveField.setTile(
			(*this << perceptiveField.getTile(perceivedSourceCoordinate)),
			perceivedSourceCoordinate
	);
	perceptiveField.setTile(
			(*this >> perceptiveField.getTile(perceivedDestinationCoordinate)),
			perceivedDestinationCoordinate
	);
}

void Ant::turnLeft() {
	Coordinate perceivedCoordinate(perceptiveField.width / 2, perceptiveField.height / 2);
	perceptiveField.setTile(*this < perceptiveField.getTile(perceivedCoordinate), perceivedCoordinate);
}

void Ant::turnRight() {
	Coordinate perceivedCoordinate(perceptiveField.width / 2, perceptiveField.height / 2);
	perceptiveField.setTile(*this > perceptiveField.getTile(perceivedCoordinate), perceivedCoordinate);
}

void Ant::eat() {
	Tile tileBeneath = perceptiveField.getTile(getLocalCoordinate());
	Energy potentialEnergy = getPotential();
	Energy energyToEat = tileBeneath.getTotalEnergy() - getTotalEnergy();
	energyToEat = (Energy) ceil(energyToEat * 0.1f);
	tileBeneath.setTotalEnergy(tileBeneath.getTotalEnergy() - energyToEat);
	perceptiveField.setTile(tileBeneath, getLocalCoordinate());
	setPotential(potentialEnergy + energyToEat);
}

void Ant::attack() {
	//Environment handles this :p
}

void Ant::fortify() {
	Energy potential = getPotential();
	Energy shieldEnergy = getShield();
	Energy transferAmount = (Energy) ceil(potential * 0.05f);
	potential -= transferAmount;
	shieldEnergy += transferAmount;
	setPotential(potential);
	setShield(shieldEnergy);
}

void Ant::randomize() {
	srand((unsigned int) time(NULL));
	const Energy HYPOTHETICAL_MAX_POTENTIAL_ENERGY = 150;
	const Energy HYPOTHETICAL_MAX_SHIELD_ENERGY = 50;
	const Energy HYPOTHETICAL_MAX_FERTILITY_ENERGY = 50;
	const Energy HYPOTHETICAL_MAX_BABY_ENERGY = 50;
	const Attitude HYPOTHETICAL_MAX_ATTITUDE = 256;
	const Attitude HYPOTHETICAL_MAX_TRAIT = 256;
	const int HYPOTHETICAL_MAX_OCCUPANCY_VAL = 4;
	Occupancy occupancy;
	int val = rand() % HYPOTHETICAL_MAX_OCCUPANCY_VAL;
	if (val == 0)
		occupancy = OCCUPANCY_NORTH;
	else if (val == 1)
		occupancy = OCCUPANCY_EAST;
	else if (val == 2)
		occupancy = OCCUPANCY_SOUTH;
	else
		occupancy = OCCUPANCY_WEST;
	setCoordinate(Coordinate(-1, -1));
	setPotential((Energy) (rand() % HYPOTHETICAL_MAX_POTENTIAL_ENERGY));
	setShield((Energy) (rand() % HYPOTHETICAL_MAX_SHIELD_ENERGY));
	setFertility((Energy) (rand() % HYPOTHETICAL_MAX_FERTILITY_ENERGY));
	setBaby((Energy) (rand() % HYPOTHETICAL_MAX_BABY_ENERGY));
	setCharacter(AgentCharacter(
			(Attitude) (rand() % HYPOTHETICAL_MAX_ATTITUDE),
			(Trait) (rand() % HYPOTHETICAL_MAX_TRAIT),
			occupancy
	));
}

void Ant::placeAntInEnvironment(Environment &environment, Coordinate coordinate) {
	environment.setTile(
			(*this >> environment.getTile(coordinate)),
			coordinate
	);
}

int Ant::calculateDistance(Coordinate c1, Coordinate c2) {
	int distanceX1 = Utils::modulo(abs(c1.getX() - c2.getX()), perceptiveField.width);
	int distanceX2 = Utils::modulo(abs(c2.getX() - c2.getX()), perceptiveField.width);
	int distanceX = min(distanceX1, distanceX2);
	int distanceY1 = Utils::modulo(abs(c1.getY() - c2.getY()), perceptiveField.height);
	int distanceY2 = Utils::modulo(abs(c2.getY() - c1.getY()), perceptiveField.height);
	int distanceY = min(distanceY1, distanceY2);
	return distanceX + distanceY;

}

unsigned long Ant::getMaxPerceptValue(percept::Percept percept) {
	unsigned long maxPerceptVal = 0;
	if (percept == percept::ENERGY) {
		Energy e = 0;
		do {
			e--;
		} while (e < 0);
		maxPerceptVal = e;
	} else if (percept == percept::ATTITUDE) {
		Attitude e = 0;
		do {
			e--;
		} while (e < 0);
		maxPerceptVal = e;
	} else if (percept == percept::TRAIT) {
		Trait e = 0;
		do {
			e--;
		} while (e < 0);
		maxPerceptVal = e;
	}
	return maxPerceptVal;
}


excitation Ant::getSensation(sensor::Sensor sensor, percept::Percept percept) {
	int maxHeight = perceptiveField.height;
	int maxWidth = perceptiveField.width;
	adjacency::Adjacency adjacency;
	if (sensor == sensor::FRONT)
		adjacency = adjacency::Adjacency::AHEAD;
	else if (sensor == sensor::LEFT)
		adjacency = adjacency::Adjacency::LEFT;
	else
		adjacency = adjacency::Adjacency::RIGHT;

	Coordinate sensoryCoordinate = getLocalCoordinate(adjacency);
	int distance;
	float totalWeightedDistance = 0;
	excitation perceivedAverage = 0;
	for (int i = 0; i < maxWidth; i++) {
		for (int j = 0; j < maxHeight; j++) {
			distance = 1 + calculateDistance(perceptiveField.getTile(Coordinate(i, j)).getGlobalCoordinate(),
											 sensoryCoordinate);
			totalWeightedDistance += (1.f / distance);
			if (percept == percept::ATTITUDE)
				perceivedAverage +=
						perceptiveField.getTile(Coordinate(i, j)).getAgentCharacter().getAttitude() * (1.f / distance);
			else if (percept == percept::TRAIT)
				perceivedAverage +=
						perceptiveField.getTile(Coordinate(i, j)).getAgentCharacter().getTrait() * (1.f / distance);
			else if (percept == percept::ENERGY)
				perceivedAverage += perceptiveField.getTile(Coordinate(i, j)).getTotalEnergy() * (1.f / distance);
		}
	}
	perceivedAverage /= totalWeightedDistance;
	float maxPerceptValue = getMaxPerceptValue(percept);
	excitation resultantExcitation = perceivedAverage / maxPerceptValue;

	return resultantExcitation;
}
