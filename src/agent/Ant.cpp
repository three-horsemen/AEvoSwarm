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

bool Ant::isActionValid(Agent::Action action) {
	switch (action) {
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
