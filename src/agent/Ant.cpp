//
// Created by reuben on 5/3/17.
//

#include <agent/Ant.hpp>

short Ant::actionCost[10] = {8, 6, 6, 2, 20, 15, 25, 15, 25, 0};

Ant::Ant(Coordinate newCoordinate, Energy newPotential, Energy newShield, Energy newFertility, Energy newBaby,
		 AgentCharacter newCharacter) {
	coordinate = newCoordinate;
	potential = newPotential;
	shield = newShield;
	fertility = newFertility;
	baby = newBaby;
	character = newCharacter;
	if (newCharacter.getOccupancy() == OCCUPANCY_DEAD)
		throw invalid_argument("An ant can't be born dead!");
}

Coordinate Ant::getCoordinateAhead(Coordinate coordinate, Occupancy occupancy) {
	int x = coordinate.getX(), y = coordinate.getY();
	switch (occupancy) {
		case OCCUPANCY_NORTH:
			y++;
			break;
		case OCCUPANCY_SOUTH:
			y--;
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
	Coordinate coordinate;
	switch ((Ant::Action) action) {
		case FORWARD:
			if (perceptiveField.getTile(
					getCoordinateAhead(getCharacter().getOccupancy())).getAgentCharacter().getOccupancy())
				return false;
			for (int direction = OCCUPANCY_NORTH;
				 direction <= ((int) OCCUPANCY_WEST); direction++) {
				coordinate = getCoordinateAhead(getCharacter().getOccupancy());
				if ((coordinate) != this->coordinate &&
					perceptiveField.getTile(coordinate).getTotalEnergy() > getTotalEnergy()) {
					return false;
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
	for (int i = -PerceptiveField::WIDTH / 2;
		 i < PerceptiveField::WIDTH / 2; i++) {
		for (int j = -PerceptiveField::HEIGHT / 2;
			 j < +PerceptiveField::HEIGHT / 2; j++) {
			int x = Utils::modulo(coordinate.getX() + i, Environment::MAX_X), y = Utils::modulo(coordinate.getY() + j,
																								Environment::MAX_Y);
			Tile tile = environment.getTile(Coordinate(x, y));
			perceptiveField.tile[i + PerceptiveField::WIDTH / 2][j + PerceptiveField::HEIGHT / 2] = tile;
		}
	}
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

void Ant::moveForward(Environment &environment) {
	Coordinate destinationCoordinate = coordinate;
	Occupancy occupancy = character.getOccupancy();
	if (occupancy == OCCUPANCY_NORTH)
		destinationCoordinate.setX(Utils::modulo((destinationCoordinate.getX() - 1), Environment::MAX_X));
	else if (occupancy == OCCUPANCY_EAST)
		destinationCoordinate.setY(Utils::modulo((destinationCoordinate.getY() + 1), Environment::MAX_Y));
	else if (occupancy == OCCUPANCY_SOUTH)
		destinationCoordinate.setX(Utils::modulo((destinationCoordinate.getX() + 1), Environment::MAX_X));
	else if (occupancy == OCCUPANCY_WEST)
		destinationCoordinate.setY(Utils::modulo((destinationCoordinate.getY() - 1), Environment::MAX_Y));
	environment.setTile(
			(*this << environment.getTile(coordinate)),
			coordinate
	);
	environment.setTile(
			(*this >> environment.getTile(destinationCoordinate)),
			destinationCoordinate
	);
}

void Ant::turnLeft(Environment &environment) {
	environment.setTile(*this < environment.getTile(coordinate), coordinate);
}

void Ant::turnRight(Environment &environment) {
	environment.setTile(*this > environment.getTile(coordinate), coordinate);
}

Ant Ant::generateRandomAnt() {
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
	Ant ant(
			Coordinate(-1, -1),
			(Energy) (rand() % HYPOTHETICAL_MAX_POTENTIAL_ENERGY),
			(Energy) (rand() % HYPOTHETICAL_MAX_SHIELD_ENERGY),
			(Energy) (rand() % HYPOTHETICAL_MAX_FERTILITY_ENERGY),
			(Energy) (rand() % HYPOTHETICAL_MAX_BABY_ENERGY),
			AgentCharacter(
					(Attitude) (rand() % HYPOTHETICAL_MAX_ATTITUDE),
					(Trait) (rand() % HYPOTHETICAL_MAX_TRAIT),
					occupancy
			)
	);
	return ant;
}

void Ant::placeAntInEnvironment(Environment &environment, Coordinate coordinate) {
	environment.setTile(
			(*this >> environment.getTile(coordinate)),
			coordinate
	);
}