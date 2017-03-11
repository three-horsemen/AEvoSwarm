//
// Created by reuben on 5/3/17.
//

#include <agent/Ant.hpp>

vector<Energy> Ant::actionCost = {8, 6, 6, 2, 20, 15, 25, 15, 25, 0};
const short Ant::actionCount = (const short) Ant::actionCost.size();
const Energy Ant::NEWBORN_POTENTIAL = (const Energy) (actionCost[FORWARD] * 20);
const Energy Ant::NEWBORN_TOTAL_ENERGY = NEWBORN_POTENTIAL + NEWBORN_SHIELD;

Ant::Ant() :
//Perceptive field size
		Agent(5, 5) {
	developBrain();

	//TODO Remove me.
	character.setAttitude((Attitude) rand());
}

Ant::Ant(const Ant &ant) : Agent(ant) {
	operator=(ant);
}

void Ant::operator=(const Ant &ant) {
	Agent::operator=(ant);
	globalCoordinate = ant.globalCoordinate;
	potential = ant.potential;
	shield = ant.shield;
	fertility = ant.fertility;
	fetal = ant.fetal;
	character = ant.character;
	if (ant.character.getOccupancy() == OCCUPANCY_DEAD)
		throw invalid_argument("An ant can't be born dead!");
	resorbBrain();
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
	return getPotential() >= actionCost[action];
}

bool Ant::isActionValid(Agent::Action agentAction) {
	if (!isEnergyAvailable(agentAction)) return false;

	Ant::Action action = (Action) agentAction;

	Coordinate coordinateAhead;
	switch (action) {
		case Ant::FORWARD:
			coordinateAhead = getLocalCoordinate(adjacency::AHEAD);
			if (perceptiveField.getTile(coordinateAhead).getAgentCharacter().getOccupancy() != OCCUPANCY_DEAD)
				return false;
			for (int adjacent = adjacency::AHEAD;
				 adjacent <= adjacency::RIGHT; adjacent++) {
				Coordinate potentialPredatorCoordinate = getCoordinate(coordinateAhead,
																	   (Occupancy) getCharacter().getOccupancy(),
																	   (adjacency::Adjacency) adjacent);
				Tile potentialPredatorTile = perceptiveField.getTile(potentialPredatorCoordinate);

				if (potentialPredatorCoordinate != getLocalCoordinate() //TODO This condition can be removed
					&& potentialPredatorTile.getAgentCharacter().getOccupancy() != OCCUPANCY_DEAD
					&& potentialPredatorTile.getTotalEnergy() >= this->getTotalEnergy()) {

					if (coordinateAhead == getCoordinate(potentialPredatorCoordinate,
														 potentialPredatorTile
																 .getAgentCharacter().getOccupancy(),
														 adjacency::AHEAD))
						return false;
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
			//TODO Also check if the baby has enough energy to be born with
			//Is there space behind me:
			return !perceptiveField.getTile(
					getLocalCoordinate(adjacency::BEHIND)).getAgentCharacter().getOccupancy()
				   && getFetal() >= NEWBORN_TOTAL_ENERGY;
		case Ant::DIE:
			return true;
		default:
			throw invalid_argument("Unknown action specified");
	}
}

void Ant::observeEnvironment(Environment &environment) {
	int x, y;
	//TODO Handle even dimensional perceptive fields
	for (int i = -(perceptiveField.height / 2); i <= +perceptiveField.height / 2; i++) {
		for (int j = -(perceptiveField.width / 2); j <= +perceptiveField.width / 2; j++) {
			x = Utils::modulo(globalCoordinate.getX() + j, environment.width);
			y = Utils::modulo(globalCoordinate.getY() + i, environment.width);
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
	sensoryInputs[ptr++] = (excitation) (log(getPotential()) / getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = (excitation) (log(getShield()) / getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = (excitation) (log(getFertility()) / getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = (excitation) (log(getFetal()) / getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = (excitation) (log(getCharacter().getAttitude()) / log(maxAttitude));
	sensoryInputs[ptr] = (excitation) (log(getCharacter().getTrait()) / log(maxTrait));
}

void Ant::selectAction() {
//	mutate();
//	Neuron::randomizeExcitation(sensoryInputs);
	brain.getLayer(0)->setInputs(sensoryInputs);
	brain.compute();
	vector<excitation> outputs = brain.getOutputLayer()->getOutputs();
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
	selectedAction = (Agent::Action) mostExcitedValidAction;;
}

Agent::Action Ant::getSelectedAction() const {
	return selectedAction;
}

void Ant::setSelectedAction(Agent::Action action, bool overrideDeath) {
	if ((selectedAction != (Agent::Action) Ant::DIE) || overrideDeath)
		selectedAction = action;
}

void Ant::performAction(Agent::Action agentAction) {
	if (!isActionValid(agentAction)) throw invalid_argument("The provided action is invalid");
	Action action = (Action) agentAction;

	potential -= actionCost[agentAction];

	dissipateEnergy(actionCost[agentAction]);

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
		case Ant::MATURE:
			mature();
			break;
		case Ant::GROW_BABY:
			growBaby();
			break;
		case Ant::GIVE_BIRTH:
			pushOutNewborn();
			break;
		case Ant::DIE:
			die();
			break;
		default:
			//TODO Complete remaining actions
//			throw invalid_argument("Undefined action selected to be performed");
			break;
	}
}

void Ant::affectEnvironment(vector<Ant> &ants, Environment &environment) {
	//TODO Handle inter-ant social interactions, like ATTACK and BIRTH.

	Energy priorEnergy = environment.getTotalEnergy();

	//Special effect of GIVE_BIRTH
	if ((Ant::Action) getSelectedAction() == Ant::GIVE_BIRTH) {
		ants.push_back(pullOutNewborn());
	}

	placeCharacterInEnvironment(*this, environment, getGlobalCoordinate());
	//Dead ants aren't placed in the environment, but may still have energy distribution effects in the environment.
	for (int x = 0; x < perceptiveField.width; x++) {
		for (int y = 0; y < perceptiveField.height; y++) {
			Tile perceptiveTile = perceptiveField.getTile(Coordinate(x, y));
			Tile globalTile = environment.getTile(perceptiveTile.getGlobalCoordinate());
			signed int differentialEnergyValue = perceptiveTile.getTotalEnergy() - globalTile.getTotalEnergy();
			if (differentialEnergyValue) {
				globalTile.setTotalEnergy((Energy) (globalTile.getTotalEnergy() + differentialEnergyValue));
				environment.setTile(globalTile, globalTile.getGlobalCoordinate());
			}
		}
	}

	//TODO Remove once confident the energy is conserved
	if (environment.getTotalEnergy() != priorEnergy) {
		cout << "Environment energy is not conserved from " << priorEnergy << " to " << environment.getTotalEnergy()
			 << " after action " << getSelectedAction() << endl;
	}
}

void Ant::eraseDeadAnts(vector<Ant> &ants) {
	for (int i = 0; i < ants.size(); i++)
		if ((ants[i].getCharacter().getOccupancy() == OCCUPANCY_DEAD) || (ants[i].getShield() <= 0)) {
			ants.erase(ants.begin() + i);
		}

}

void Ant::realizeAntsAction(vector<Ant> &ants, Environment &environment) {
	environment.clearCharacterGrid();
	unsigned long currentAntCount = ants.size(); //This may change if ants are born, or if they die.
	for (int i = 0; i < currentAntCount; i++) {
//		environment.setTile((ants[i] << environment.getTile(ants[i].getGlobalCoordinate()), ants[i].getGlobalCoordinate());
		ants[i].affectEnvironment(ants, environment);
	}
	eraseDeadAnts(ants);
}

Ant Ant::pullOutNewborn() {
	if (bornFetalEnergy < NEWBORN_TOTAL_ENERGY) {
		throw runtime_error("Can only spawn one newborn after one birth");
	}
	Ant newBorn(*this);

	newBorn.setShield(NEWBORN_SHIELD);
	newBorn.setPotential(bornFetalEnergy - NEWBORN_SHIELD);

	bornFetalEnergy = (Energy) 0;

	newBorn.mutate();
	return newBorn;
}

void Ant::mutate() {
	for (int i = 0; i < brain.getDepth(); i++) {
		if (brain.getLayer(i)->type == FULLY_CONNECTED) {
			FullyConnectedLayer *layer = (FullyConnectedLayer *) brain.getLayer(i);
			for (int j = 0; j < layer->outputSize; j++) {
				vector<weight> weights = layer->getNeuronWeights(j);
				Neuron::mutateWeights(weights, 0.06f);
				layer->setNeuronWeights(j, weights);
			}
		}
	}
	//TODO Update trait here
}

void Ant::developBrain() {
	const short inputSize = senseCount + memoryCount;
	const short fC1Size = (senseCount + memoryCount) * 9 / 10;
	const short fC2Size = (senseCount + memoryCount) * 81 / 100;
	const short outputSize = actionCount + memoryCount;

	if (brain.getDepth() > 0) {
		throw runtime_error("Cannot develop already developed brain");
	}

	InputLayer inputLayer(inputSize);
	brain.addLayer((Layer &) inputLayer);

	vector<vector<weight> > weights1((unsigned long) fC1Size, vector<weight>((unsigned long) inputSize));
	Neuron::randomizeWeights(weights1);
	FullyConnectedLayer fullyConnectedLayer1(weights1);
	brain.addLayer((Layer &) fullyConnectedLayer1);

	vector<vector<weight> > weights2((unsigned long) fC2Size, vector<weight>((unsigned long) fC1Size));
	Neuron::randomizeWeights(weights2);
	FullyConnectedLayer fullyConnectedLayer2(weights2);
	brain.addLayer((Layer &) fullyConnectedLayer2);

	vector<vector<weight> > weights3((unsigned long) outputSize, vector<weight>((unsigned long) fC2Size));
	Neuron::randomizeWeights(weights3);
	FullyConnectedLayer fullyConnectedLayer3(weights3);
	brain.addLayer((Layer &) fullyConnectedLayer3);

	OutputLayer outputLayer(outputSize);
	brain.addLayer((Layer &) outputLayer);

	sensoryInputs = vector<excitation>((unsigned long) inputSize);
	brain.compute();//TODO Remove me
}

void Ant::resorbBrain() {
	brain.resorb();
}

Coordinate Ant::getGlobalCoordinate(Occupancy occupancy, adjacency::Adjacency adjacency) {
	return getCoordinate(globalCoordinate, occupancy, adjacency);
}

Coordinate Ant::getGlobalCoordinate(adjacency::Adjacency adjacency) {
	return getCoordinate(globalCoordinate, character.getOccupancy(), adjacency);
}

Coordinate Ant::getGlobalCoordinate() {
	return globalCoordinate;
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

Energy Ant::getFetal() {
	return fetal;
}

AgentCharacter Ant::getCharacter() {
	return character;
}

void Ant::setCoordinate(Coordinate newCoordinate) {
	globalCoordinate = newCoordinate;
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

void Ant::setFetal(Energy newFetal) {
	fetal = newFetal;
}

void Ant::setCharacter(AgentCharacter newCharacter) {
	character.setAttitude(newCharacter.getAttitude());
	character.setOccupancy(newCharacter.getOccupancy());
	character.setTrait(newCharacter.getTrait());
}

Energy Ant::getTotalEnergy() {
	return potential + shield + fertility + fetal;
}

Tile Ant::operator<<(Tile tile) {
	tile.setAgentCharacter(AgentCharacter());
	tile.setTotalEnergy(tile.getTotalEnergy() - this->getTotalEnergy());
	return tile;
}

Tile Ant::operator>>(Tile tile) {
	tile.setAgentCharacter(character);
	tile.setTotalEnergy(tile.getTotalEnergy() + this->getTotalEnergy());
	globalCoordinate = tile.getGlobalCoordinate();
	return tile;
}

Tile Ant::operator<=(Tile tile) {
	tile.setAgentCharacter(AgentCharacter());
	return tile;
}

Tile Ant::operator>=(Tile tile) {
	tile.setAgentCharacter(character);
	globalCoordinate = tile.getGlobalCoordinate();
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
	return tile;
}

void Ant::dissipateEnergy(Energy energy) {
	Tile tile = perceptiveField.getTile(getLocalCoordinate());
	tile.setTotalEnergy(tile.getTotalEnergy() - energy);
	perceptiveField.setTile(tile, getLocalCoordinate());

	tile = perceptiveField.getTile(getLocalCoordinate(adjacency::BEHIND));
	tile.setTotalEnergy(tile.getTotalEnergy() + energy);
	perceptiveField.setTile(tile, getLocalCoordinate(adjacency::BEHIND));
}

void Ant::moveForward() {
	Coordinate localCoordinate = getLocalCoordinate();
	Coordinate destinationCoordinate = getLocalCoordinate(adjacency::AHEAD);

	perceptiveField.setTile(
			(*this << perceptiveField.getTile(localCoordinate)),
			localCoordinate
	);
	perceptiveField.setTile(
			(*this >> perceptiveField.getTile(destinationCoordinate)),
			destinationCoordinate
	);
}

void Ant::turnLeft() {
	Coordinate localCoordinate = getLocalCoordinate();
	perceptiveField.setTile(*this < perceptiveField.getTile(localCoordinate), localCoordinate);
}

void Ant::turnRight() {
	Coordinate localCoordinate = getLocalCoordinate();
	perceptiveField.setTile(*this > perceptiveField.getTile(localCoordinate), localCoordinate);
}

void Ant::eat() {
	Tile tileBeneath = perceptiveField.getTile(getLocalCoordinate());
	Energy energyToEat = (Energy) ceil((tileBeneath.getTotalEnergy() - getTotalEnergy()) * 0.1f);
	setPotential(getPotential() + energyToEat);
}

void Ant::attack() {
	//Environment handles this :p
}

void Ant::beAttacked(Energy damage) {
	damage = min(getShield(), damage);
	setShield(getShield() - damage);

	dissipateEnergy(damage);

	assert(getShield() >= 0);

	if (getShield() == 0) {
		die();
	}

}

void Ant::fortify() {
	Energy potential = getPotential();
	Energy shieldEnergy = getShield();
	Energy transferAmount = (Energy) ceil(potential * FORTIFY_FACTOR);
	potential -= transferAmount;
	shieldEnergy += transferAmount;
	setPotential(potential);
	setShield(shieldEnergy);
}

void Ant::mature() {
	Energy potential = getPotential();
	Energy fertilityEnergy = getFertility();
	Energy transferAmount = (Energy) ceil(potential * MATURE_FACTOR);
	potential -= transferAmount;
	fertilityEnergy += transferAmount;
	setPotential(potential);
	setFertility(fertilityEnergy);
}

void Ant::growBaby() {
	Energy potential = getPotential();
	Energy fetalEnergy = getFetal();
	Energy transferAmount = (Energy) ceil(potential * FETAL_DEVELOPMENT_FACTOR);
	potential -= transferAmount;
	fetalEnergy += transferAmount;
	setPotential(potential);
	setFetal(fetalEnergy);
}

void Ant::pushOutNewborn() {
	if (bornFetalEnergy != 0) {
		throw runtime_error("Cannot push new born until previous is pulled out");
	}
	bornFetalEnergy = getFetal();
	setFetal(0);
}

void Ant::die() {
	setPotential(0);
	setShield(0);
	setFertility(0);
	setFetal(0);
	dissipateEnergy(getTotalEnergy());

	character.setOccupancy(OCCUPANCY_DEAD);
	character.setAttitude(GROUND_ATTITUDE);
	character.setTrait(GROUND_TRAIT);
}

void Ant::randomize() {
	srand((unsigned int) time(NULL));
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
	setFetal((Energy) (rand() % HYPOTHETICAL_MAX_BABY_ENERGY));
	setCharacter(AgentCharacter(
			(Attitude) ((rand() % (HYPOTHETICAL_MAX_ATTITUDE - HYPOTHETICAL_MIN_ATTITUDE)) + HYPOTHETICAL_MIN_ATTITUDE),
			(Trait) (rand() % HYPOTHETICAL_MAX_TRAIT),
			occupancy
	));
}

void Ant::placeInEnvironment(Ant &ant, Environment &environment, Coordinate coordinate) {
	environment.setTile(
			(ant >> environment.getTile(coordinate)),
			coordinate
	);
}

void Ant::placeCharacterInEnvironment(Ant &ant, Environment &environment, Coordinate coordinate) {
	environment.setTile(
			(ant >= environment.getTile(coordinate)),
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

excitation Ant::getMaxPerceptValue(percept::Percept percept) {
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
	return (excitation) log(maxPerceptVal);
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
			distance = 1 + calculateDistance(Coordinate(i, j), sensoryCoordinate);
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
	perceivedAverage = (excitation) log(perceivedAverage + exp(1.0)); //Trying to make excitation logarithmic.
	excitation resultantExcitation = (perceivedAverage / getMaxPerceptValue(percept));
	return resultantExcitation;
}
