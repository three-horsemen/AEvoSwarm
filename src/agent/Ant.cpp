//
// Created by reuben on 5/3/17.
//

#include <agent/Ant.hpp>

//								FORWARD LEFT RIGHT EAT ATTACK FORTIFY MATURE GROW_BABY GIVE_BIRTH DIE
vector<Energy> Ant::actionCost = {5, 4, 4, 4, 20, 15, 25, 5, 5, 0};
const short Ant::actionCount = (const short) Ant::actionCost.size();
const Energy Ant::NEWBORN_MIN_POTENTIAL = (const Energy) (actionCost[FORWARD] * 20);
const Energy Ant::NEWBORN_MIN_TOTAL_ENERGY = NEWBORN_MIN_POTENTIAL + NEWBORN_MIN_SHIELD;

Ant::Ant() :
//Perceptive field size
		Agent(5, 5) {
	developBrain();

	age = 0;
	pushedFetalEnergy = 0;
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
	pushedFetalEnergy = ant.pushedFetalEnergy;
	age = ant.age;
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

bool Ant::isInImpactRange(Environment &environment, Coordinate coordinate) {
//TODO Look here for bug bounty.
	for (int adjacent = adjacency::AHEAD;
		 adjacent <= adjacency::RIGHT; adjacent++) {
		Coordinate potentialPredatorCoordinate = getGlobalCoordinate(environment, coordinate,
																	 (Occupancy) OCCUPANCY_NORTH,
																	 (adjacency::Adjacency) adjacent);//TODO Optimization: Directly access north, south, east, west tiles
		if (environment
					.getTile(potentialPredatorCoordinate)
					.getAgentCharacter()
					.getOccupancy() != OCCUPANCY_DEAD
			&& (coordinate == getCoordinate(potentialPredatorCoordinate,
											environment
													.getTile(potentialPredatorCoordinate)
													.getAgentCharacter().getOccupancy(),
											adjacency::AHEAD)
				|| coordinate == getCoordinate(potentialPredatorCoordinate,
											   environment
													   .getTile(potentialPredatorCoordinate)
													   .getAgentCharacter().getOccupancy(),
											   adjacency::BEHIND)))
			return true;
	}
	return false;
}

bool Ant::isThereBirthSpace() {
	if (getFetal() < NEWBORN_MIN_TOTAL_ENERGY)
		return false;
	if (perceptiveField.getTile(getLocalCoordinate(adjacency::BEHIND)).getAgentCharacter().getOccupancy() !=
		OCCUPANCY_DEAD)
		return false;
	return !isInImpactRange(perceptiveField, getLocalCoordinate(
			adjacency::BEHIND));
}


bool Ant::isEnergyAvailable(Agent::Action action) {
	return getPotential() >= getActionCost(action);
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
			return isThereBirthSpace();
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
	sensoryInputs[ptr++] = getEnergyExcitation(getPotential(), (Energy) getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = getEnergyExcitation(getShield(), (Energy) getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = getEnergyExcitation(getFertility(), (Energy) getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = getEnergyExcitation(getFetal(), (Energy) getMaxPerceptValue(percept::ENERGY));
	sensoryInputs[ptr++] = (excitation) (log(getCharacter().getAttitude()) / log(maxAttitude));
	sensoryInputs[ptr] = (excitation) (log(getCharacter().getTrait()) / log(maxTrait));
}

void Ant::selectAction() {
//	mutate();
//	Neuron::randomizeExcitation(sensoryInputs);
	assert(brain.getLayer(0)->inputSize == sensoryInputs.size());
	sensoryInputs[sensoryInputs.size() - 1] = 1;
	brain.getLayer(0)->setInputs(sensoryInputs);
	brain.compute();
	vector<excitation> outputs = brain.getOutputLayer()->getOutputs();

	for (int i = 0; i < memoryCount; i++) {
		sensoryInputs[senseCount + i] = outputs[actionCount + i];
	}

	assert(brain.getOutputLayer()->outputSize == actionCount + memoryCount);

	outputs[DIE] = -1;

//	for (int i = 0; i < outputs.size(); i++) {
//		cout<<outputs[i]<<' ';
//	}
//	cout<<endl;
	int mostExcitedValidAction = -1;
	float maxExcitation = -2;
	for (int action = 0; action < actionCount; action++) {
		if (isActionValid((Agent::Action) action) && outputs[action] > maxExcitation) {
			mostExcitedValidAction = action;
			maxExcitation = outputs[action];
		}
	}

	assert(mostExcitedValidAction != -1);
//	assert(getPotential() > 0 && mostExcitedValidAction != DIE); TODO Ensure that the only time an ant picks death is if no other option is available
	selectedAction = (Agent::Action) mostExcitedValidAction;
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

	potential -= getActionCost(agentAction);

	age++;

	dissipateEnergy(getActionCost(agentAction));

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

void Ant::affectEnvironment(vector<Ant> &ants, unsigned short indexOfAnt, Environment &oldEnvironment,
							Environment &newEnvironment) {
	assert(indexOfAnt < ants.size());

	//TODO Handle inter-ant social interactions, like ATTACK and BIRTH.

//	Energy priorEnergy = oldEnvironment.getTotalEnergy();

	//Special effect of GIVE_BIRTH
	if ((Ant::Action) ants[indexOfAnt].getSelectedAction() == Ant::GIVE_BIRTH) {
		Ant newborn;
		ants[indexOfAnt].pullOutNewborn(newEnvironment, newborn);
		placeInEnvironment(newborn, newEnvironment, newborn.getGlobalCoordinate());
		cout << "Newborn placed at (" << newborn.getGlobalCoordinate().getX() << ","
			 << newborn.getGlobalCoordinate().getY() << ")" << endl;
		ants.push_back(newborn);
	}

	placeCharacterInEnvironment(ants[indexOfAnt], newEnvironment, ants[indexOfAnt].getGlobalCoordinate());
	//Dead ants aren't placed in the environment, but may still have energy distribution effects in the environment.

	//Right now this is just updating the perceived energy changes.
	for (int x = 0; x < ants[indexOfAnt].getPerceptiveField()->width; x++) {
		for (int y = 0; y < ants[indexOfAnt].getPerceptiveField()->height; y++) {
			Tile perceptiveTile = ants[indexOfAnt].getPerceptiveField()->getTile(Coordinate(x, y));
			Tile globalTile = newEnvironment.getTile(perceptiveTile.getGlobalCoordinate());
			signed int differentialEnergyValue = perceptiveTile.getTotalEnergy() - oldEnvironment.getTile(
					perceptiveTile.getGlobalCoordinate()).getTotalEnergy();
			if (differentialEnergyValue) {
				globalTile.setTotalEnergy((Energy) (globalTile.getTotalEnergy() + differentialEnergyValue));
				newEnvironment.setTile(globalTile, globalTile.getGlobalCoordinate());
			}
		}
	}
}

void Ant::eraseDeadAnts(vector<Ant> &ants) {
	for (int i = 0; i < ants.size(); i++)
		if ((ants[i].getCharacter().getOccupancy() == OCCUPANCY_DEAD) || (ants[i].getShield() <= 0)) {
			ants.erase(ants.begin() + i);
			i--;
		}

}

void Ant::realizeAntsAction(vector<Ant> &ants, Environment &environment) {
	Environment environmentBackup(environment);
	environment.clearCharacterGrid();
	unsigned long currentAntCount = ants.size(); //This may change if ants are born, or if they die.
	for (unsigned short i = 0; i < currentAntCount; i++) {
//		environment.setTile((ants[i] << environment.getTile(ants[i].getGlobalCoordinate()), ants[i].getGlobalCoordinate());
		affectEnvironment(ants, i, environmentBackup, environment);
	}
	eraseDeadAnts(ants);
}

void Ant::pullOutNewborn(Environment &environment, Ant &newBorn) {
	if (pushedFetalEnergy < NEWBORN_MIN_TOTAL_ENERGY) {
		throw runtime_error("Can only spawn one newborn after one birth");
	}
	newBorn = *this;

	newBorn.setShield(NEWBORN_MIN_SHIELD);
	newBorn.setPotential(pushedFetalEnergy - NEWBORN_MIN_SHIELD);
	newBorn.setFetal(0);
	newBorn.setFertility(0);
	newBorn.setPushedFetalEnergy(0);
	newBorn.setGlobalCoordinate(getGlobalCoordinate(environment, adjacency::BEHIND));

	pushedFetalEnergy = (Energy) 0;

	newBorn.mutate();
}

void Ant::mutate() {
//	cout << "Ant MUTATED\n";
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
	const short inputSize = senseCount + memoryCount + 1;
//	const short fC1Size = (senseCount + memoryCount) * 9 / 10;
//	const short fC2Size = (senseCount + memoryCount) * 81 / 100;
	const short outputSize = actionCount + memoryCount;

	if (brain.getDepth() > 0) {
		throw runtime_error("Cannot develop already developed brain");
	}

	InputLayer inputLayer(inputSize);
	brain.addLayer((Layer &) inputLayer);

	vector<vector<weight>> weights1((unsigned long) outputSize, vector<weight>((unsigned long) inputSize));
	Neuron::randomizeWeights(weights1);
	FullyConnectedLayer fullyConnectedLayer1(weights1);
	brain.addLayer((Layer &) fullyConnectedLayer1);

//	vector<vector<weight>> weights2((unsigned long) fC2Size, vector<weight>((unsigned long) fC1Size));
//	Neuron::randomizeWeights(weights2);
//	FullyConnectedLayer fullyConnectedLayer2(weights2);
//	brain.addLayer((Layer &) fullyConnectedLayer2);
//
//	vector<vector<weight>> weights3((unsigned long) outputSize, vector<weight>((unsigned long) fC2Size));
//	Neuron::randomizeWeights(weights3);
//	FullyConnectedLayer fullyConnectedLayer3(weights3);
//	brain.addLayer((Layer &) fullyConnectedLayer3);

	OutputLayer outputLayer(outputSize);
	brain.addLayer((Layer &) outputLayer);

	sensoryInputs = vector<excitation>((unsigned long) inputSize);
	brain.compute();//TODO Remove me
}

void Ant::resorbBrain() {
	brain.resorb();
}

Coordinate Ant::getGlobalCoordinate(Environment &environment, Coordinate coordinate, Occupancy occupancy,
									adjacency::Adjacency adjacency) {
	Coordinate potentialOutOfBoundsCoordinate = getCoordinate(coordinate, occupancy, adjacency);
	int x = potentialOutOfBoundsCoordinate.getX();
	int y = potentialOutOfBoundsCoordinate.getY();
	potentialOutOfBoundsCoordinate.setX(Utils::modulo(x, environment.width));
	potentialOutOfBoundsCoordinate.setY(Utils::modulo(y, environment.height));
	return potentialOutOfBoundsCoordinate;
}

Coordinate Ant::getGlobalCoordinate(Environment &environment, Occupancy occupancy, adjacency::Adjacency adjacency) {
	Coordinate potentialOutOfBoundsCoordinate = getCoordinate(globalCoordinate, occupancy, adjacency);
	int x = potentialOutOfBoundsCoordinate.getX();
	int y = potentialOutOfBoundsCoordinate.getY();
	potentialOutOfBoundsCoordinate.setX(Utils::modulo(x, environment.width));
	potentialOutOfBoundsCoordinate.setY(Utils::modulo(y, environment.height));
	return potentialOutOfBoundsCoordinate;
}

Coordinate Ant::getGlobalCoordinate(Environment &environment, adjacency::Adjacency adjacency) {
	return getGlobalCoordinate(environment, character.getOccupancy(), adjacency);
}

Coordinate Ant::getGlobalCoordinate() {
	return globalCoordinate;
}

inline Energy Ant::getActionCost(Agent::Action action) {
//	return actionCost[action];
	return (Energy) (actionCost[action] *
					 (1.f + 5.f * ((float) getTotalEnergy() / getMaxPerceptValue(percept::ENERGY))) *
					 (1.f + 1000000.f * (((float) age / ((unsigned int) -1)))));
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

inline AgentCharacter Ant::getCharacter() {
	return character;
}

void Ant::setGlobalCoordinate(Coordinate newCoordinate) {
	globalCoordinate = newCoordinate;
}

inline void Ant::setPotential(Energy newPotential) {
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

void Ant::setPushedFetalEnergy(Energy newPushedFetalEnergy) {
	pushedFetalEnergy = newPushedFetalEnergy;
}

inline void Ant::setCharacter(AgentCharacter newCharacter) {
	character.setAttitude(newCharacter.getAttitude());
	character.setOccupancy(newCharacter.getOccupancy());
	character.setTrait(newCharacter.getTrait());
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

	//TODO Do this for other such actions
	if (shieldEnergy < getShield()) {
		shieldEnergy = (Energy) -1;
	}
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
	if (pushedFetalEnergy != 0) {
		throw runtime_error("Cannot push new born until previous is pulled out");
	}
	pushedFetalEnergy = getFetal();

	Coordinate localCoordinate = getLocalCoordinate();
	Tile tile = perceptiveField.getTile(localCoordinate);
	tile.setTotalEnergy(tile.getTotalEnergy() - pushedFetalEnergy);
	perceptiveField.setTile(tile, localCoordinate);

	//Putting energy in the tile behind is the responsibility of the environment
//	localCoordinate = getLocalCoordinate(adjacency::BEHIND);
//	tile = perceptiveField.getTile(localCoordinate);
//	tile.setTotalEnergy(tile.getTotalEnergy() + pushedFetalEnergy);
//	perceptiveField.setTile(tile, localCoordinate);

	setFetal(0);
}

void Ant::die() {
	dissipateEnergy(getTotalEnergy());
	setPotential(0);
	setShield(0);
	setFertility(0);
	setFetal(0);

	character.setOccupancy(OCCUPANCY_DEAD);
	character.setAttitude(GROUND_ATTITUDE);
	character.setTrait(GROUND_TRAIT);
}

void Ant::randomize() {
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
	setGlobalCoordinate(Coordinate(-1, -1));
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
//	unsigned long maxPerceptVal = 0;
	if (percept == percept::ENERGY) {
		return (Energy) -1;
//		Energy e = 0;
//		do {
//			e--;
//		} while (e < 0);
//		maxPerceptVal = e;
	} else if (percept == percept::ATTITUDE) {
		return (Attitude) -1;
//		Attitude e = 0;
//		do {
//			e--;
//		} while (e < 0);
//		maxPerceptVal = e;
	} else if (percept == percept::TRAIT) {
		return (Trait) -1;
//		Trait e = 0;
//		do {
//			e--;
//		} while (e < 0);
//		maxPerceptVal = e;
	} else {
		throw invalid_argument("Unknown percept");
	}
//	return (excitation) maxPerceptVal;
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
	return (log(perceivedAverage + 1) / log(getMaxPerceptValue(percept) + 1)) - 1;
}

excitation Ant::getEnergyExcitation(Energy energy, Energy maxEnergy) {
	return (excitation) (log(energy + 1) / log(maxEnergy + 1)) - 1;
}

void Ant::sparkLifeAt(Environment &environment, vector<Ant> &ants, Ant &ant) {
	Tile tile = environment.getTile(ant.getGlobalCoordinate());
	if (tile.getAgentCharacter().getOccupancy() == OCCUPANCY_DEAD
		&& tile.getTotalEnergy() >= ant.getTotalEnergy()
		&& !Ant::isInImpactRange(environment, tile.getGlobalCoordinate())
			) {

		Ant::placeCharacterInEnvironment(ant, environment, ant.getGlobalCoordinate());

		ants.push_back(ant);
//		cout << "Spawned ant at " << coordinate.toString() << endl;
	} else {
		throw invalid_argument("Cannot spark life at " + ant.getGlobalCoordinate().toString());
	}
}

bool Ant::sparkLifeWhereAvailable(Environment &environment, vector<Ant> &ants, Ant &ant) {
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
				Ant::sparkLifeAt(environment, ants, ant);
				return true;
			}
		}
	}
	return false;
}

void Ant::sparkNLives(Environment &environment, vector<Ant> &ants, unsigned int count) {
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
				ant.setFertility(0);
				ant.setFetal(0);
				ant.setPushedFetalEnergy(0);
				ant.setShield(Ant::NEWBORN_MIN_SHIELD);
				ant.setPotential(tile.getTotalEnergy() - Ant::NEWBORN_MIN_SHIELD);
				ant.setGlobalCoordinate(Coordinate(X, Y));
				Ant::sparkLifeAt(environment, ants, ant);
				count--;
			}
		}
	}
	//TODO Return false
}

void Ant::save(ostream &file, vector<Ant> &ants) {
	file <= ants.size();
	for (unsigned long i = 0; i < ants.size(); i++) {
		ants[i].save(file);
	}
}

bool Ant::load(istream &file, Environment &environment, vector<Ant> &ants) {
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
		cout << e.what() << endl;
		return false;
	}
}

void Ant::save(ostream &file) {
	file <= globalCoordinate.getX() <= globalCoordinate.getY()
	<= potential <= shield <= fertility <= fetal
	<= age;
	brain.save(file);
}

void Ant::load(istream &file, Environment &environment) {
	int X, Y;
	file >= X >= Y;
	globalCoordinate.setX(X);
	globalCoordinate.setY(Y);

	file >= potential;
	file >= shield;
	file >= fertility;
	file >= fetal;
	file >= age;

//	cout << "Loaded " << X << Y << endl;

	AgentCharacter character = environment.getTile(globalCoordinate).getAgentCharacter();
	setCharacter(character);
	brain.load(file);
}

void Ant::saveWithCharacter(ostream &file) {
	file <= globalCoordinate.getX() <= globalCoordinate.getY()
	<= potential <= shield <= fertility <= fetal
	<= age
	<= getCharacter().getAttitude()
	<= getCharacter().getOccupancy()
	<= getCharacter().getTrait();
	brain.save(file);
}

void Ant::loadWithCharacter(istream &file) {
	int X, Y;
	file >= X >= Y;

	globalCoordinate.setX(X);
	globalCoordinate.setY(Y);
//	cout << "Loaded coordinates " << globalCoordinate.toString() << endl;

	file >= potential >= shield >= fertility >= fetal
	>= age;

	Attitude attitude;
	Occupancy occupancy;
	Trait trait;
	file >= attitude
	>= occupancy
	>= trait;

	setCharacter(AgentCharacter(attitude, trait, occupancy));

	brain.load(file);
}
