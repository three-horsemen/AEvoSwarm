//
// Created by reuben on 5/3/17.
//

#include <agent/Ant.hpp>

short Ant::actionCost[10] = {8, 6, 6, 2, 20, 15, 25, 15, 25, 0};

Ant::Ant() : Agent(5, 5) {
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

Coordinate Ant::getCoordinate(Coordinate coordinate, Occupancy occupancy, adjacency::Adjacency adjacency) {
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

Coordinate Ant::getCoordinate(Occupancy occupancy, adjacency::Adjacency adjacency) {
	return getCoordinate(coordinate, occupancy, adjacency);
}

Coordinate Ant::getCoordinate(adjacency::Adjacency adjacency) {
	return getCoordinate(coordinate, character.getOccupancy(), adjacency);
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
					getCoordinate(adjacency::AHEAD)).getAgentCharacter().getOccupancy() !=
				OCCUPANCY_DEAD)
				return false;
			coordinateAhead = getCoordinate(adjacency::AHEAD);
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
			return perceptiveField.getTile(this->coordinate).getTotalEnergy() > getTotalEnergy();
		case Ant::ATTACK:
			return perceptiveField.getTile(
					getCoordinate(adjacency::AHEAD)).getAgentCharacter().getOccupancy();
		case Ant::FORTIFY:
		case Ant::MATURE:
		case Ant::GROW_BABY:
			return true;
		case Ant::GIVE_BIRTH:
			return !perceptiveField.getTile(
					getCoordinate(adjacency::BEHIND)).getAgentCharacter().getOccupancy();
		case Ant::DIE:
			return getPotential() > actionCost[Ant::EAT];
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

Agent::Action Ant::getSelectAction() {
	//TODO Use brain and senses here
}

Agent::Action Ant::performAction(Agent::Action agentAction) {
	Action action = (Action) agentAction;
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
		default:
			break;
	}
}

void Ant::affectEnvironment(Environment &) {
	//TODO Turn ant fantasies into reality
}

void Ant::developBrain() {
	const short inputSize = 25, fC1Size = 24, fC2Size = 16, outputSize = 10;
	InputLayer inputLayer(inputSize);
	excitation excitations[inputSize];
	neuron::randomizeExcitation(inputSize, excitations);
	inputLayer.setInputs((excitation *) excitations);
	brain.addLayer((Layer &) inputLayer);

	weight weights1[fC1Size][inputSize];
	neuron::randomizeWeights(inputSize, fC1Size, (weight *) weights1);
	FullyConnectedLayer fullyConnectedLayer1(inputSize, fC1Size, (weight *) weights1);
	brain.addLayer((Layer &) fullyConnectedLayer1);

	weight weights2[fC2Size][fC1Size];
	neuron::randomizeWeights(fC1Size, fC2Size, (weight *) weights2);
	FullyConnectedLayer fullyConnectedLayer2(fC1Size, fC2Size, (weight *) weights2);
	brain.addLayer((Layer &) fullyConnectedLayer2);

	weight weights3[outputSize][fC2Size];
	neuron::randomizeWeights(fC2Size, outputSize, (weight *) weights3);
	FullyConnectedLayer fullyConnectedLayer3(fC2Size, outputSize, (weight *) weights3);
	brain.addLayer((Layer &) fullyConnectedLayer3);

	OutputLayer outputLayer(outputSize);
	brain.addLayer((Layer &) outputLayer);
}

Coordinate Ant::getCoordinate() {
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
	coordinate = tile.getCoordinate();
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

void Ant::randomize() {
	srand((unsigned int) time(NULL));
	const Energy HYPOTHETICAL_MAX_POTENTIAL_ENERGY = 50;
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

	Coordinate sensoryCoordinate = getCoordinate(adjacency);
	int distance;
	float totalWeightedDistance = 0;
	float perceivedAverage = 0;
	for (int i = 0; i < maxWidth; i++) {
		for (int j = 0; j < maxHeight; j++) {
			distance = 1 + calculateDistance(perceptiveField.getTile(Coordinate(i, j)).getCoordinate(),
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
	return perceivedAverage;
}
