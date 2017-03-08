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

Coordinate Ant::getCoordinateAhead(Coordinate coordinate, Occupancy occupancy) {
	int x = coordinate.getX(), y = coordinate.getY();
	switch (occupancy) {
		case OCCUPANCY_NORTH:
			y--;
			break;
		case OCCUPANCY_SOUTH:
			y++;
			break;
		case OCCUPANCY_EAST:
			x++;
			break;
		case OCCUPANCY_WEST:
			x--;
			break;
		default:
			throw invalid_argument("Unknown occupancy specified");
	}
	return Coordinate(x, y);
}

Coordinate Ant::getCoordinateAhead(Occupancy occupancy) {
	return getCoordinateAhead(coordinate, occupancy);
}

Coordinate Ant::getCoordinateAhead() {
	return getCoordinateAhead(coordinate, getCharacter().getOccupancy());
}

Coordinate Ant::getCoordinateBehind(Coordinate coordinate, Occupancy occupancy) {
	return getCoordinateAhead(coordinate, (Occupancy) (occupancy ^ 1));
}

Coordinate Ant::getCoordinateBehind(Occupancy occupancy) {
	return getCoordinateAhead(coordinate, (Occupancy) (occupancy ^ 1));
}

bool Ant::isEnergyAvailable(Agent::Action action) {
	return getPotential() > actionCost[action];
}

bool Ant::isActionValid(Agent::Action action) {
	if (!isEnergyAvailable(action)) return false;

	Coordinate coordinateAhead;
	switch ((Ant::Action) action) {
		case FORWARD:
			if (perceptiveField.getTile(
					getCoordinateAhead(getCharacter().getOccupancy())).getAgentCharacter().getOccupancy() !=
				OCCUPANCY_DEAD)
				return false;
			coordinateAhead = getCoordinateAhead();
			for (int direction = OCCUPANCY_NORTH;
				 direction <= ((int) OCCUPANCY_WEST); direction++) {
				Coordinate potentialPredatorCoordinate = getCoordinateAhead(coordinateAhead, (Occupancy) direction);
				Tile potentialPredatorTile = perceptiveField.getTile(potentialPredatorCoordinate);

				if (potentialPredatorCoordinate != this->coordinate
					&& potentialPredatorTile.getAgentCharacter().getOccupancy() != OCCUPANCY_DEAD) {
					Coordinate rangeOfImpactCoordinate = getCoordinateAhead(potentialPredatorCoordinate,
																			potentialPredatorTile
																					.getAgentCharacter().getOccupancy());
					if (rangeOfImpactCoordinate == coordinateAhead &&
						potentialPredatorTile.getTotalEnergy() >= this->getTotalEnergy()) {
						return false;
					}
				}
			}
			return true;
		case LEFT:
		case RIGHT:
			return true;
		case EAT:
			return perceptiveField.getTile(this->coordinate).getTotalEnergy() > getTotalEnergy();
		case ATTACK:
			return perceptiveField.getTile(
					getCoordinateAhead(getCharacter().getOccupancy())).getAgentCharacter().getOccupancy();
		case FORTIFY:
		case MATURE:
		case GROW_BABY:
			return true;
		case GIVE_BIRTH:
			return !perceptiveField.getTile(
					getCoordinateBehind(getCharacter().getOccupancy())).getAgentCharacter().getOccupancy();
		case DIE:
			return getPotential() > actionCost[EAT];
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
	Coordinate perceivedDestinationCoordinate = getCoordinateAhead(perceivedSourceCoordinate,
																   getCharacter().getOccupancy());

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