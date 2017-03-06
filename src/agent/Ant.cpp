//
// Created by reuben on 5/3/17.
//

#include <agent/Ant.hpp>

Ant::Ant(Coordinate newCoordinate, Energy newPotential, Energy newShield, Energy newFertility, Energy newBaby,
		 AgentCharacter newCharacter) {
	coordinate = newCoordinate;
	potential = newPotential;
	shield = newShield;
	fertility = newFertility;
	baby = newBaby;
	character = newCharacter;
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
	for (int i = coordinate.getX() - PerceptiveField::WIDTH / 2;
		 i < coordinate.getX() + PerceptiveField::WIDTH / 2; i++) {
		for (int j = coordinate.getY() - PerceptiveField::HEIGHT / 2;
			 j < coordinate.getY() + PerceptiveField::HEIGHT / 2; j++) {
			int x = Utils::modulo(i, MAP_MAX_X), y = Utils::modulo(j, MAP_MAX_Y);
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
	std::cout << "Occupancy of tile before withdrawal: " << tile.getAgentCharacter().getOccupancy() << endl;
	tile.setAgentCharacter(AgentCharacter());
	std::cout << "Occupancy of tile after withdrawal: " << tile.getAgentCharacter().getOccupancy() << endl;
	tile.setTotalEnergy(tile.getTotalEnergy() - this->getTotalEnergy());
	return tile;
}

Tile Ant::operator>>(Tile tile) {
	tile.setAgentCharacter(this->getCharacter());
	tile.setTotalEnergy(tile.getTotalEnergy() + this->getTotalEnergy());
	this->setCoordinate(tile.getCoordinate());
	return tile;
}
