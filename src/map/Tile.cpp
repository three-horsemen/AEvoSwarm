//
// Created by soorya on 5/3/17.
//

#include "map/Tile.hpp"

//Tile::Tile(Tile &newTile) :
//		coordinate(newTile.getCoordinate().getX(), newTile.getCoordinate().getY()),
//		antCharacter(newTile.getAntCharacter().getAttitude(), newTile.getAntCharacter().getTrait(),
//					 newTile.getAntCharacter().getOccupancy()) {
//	coordinate = newTile.getCoordinate();
//	antCharacter = newTile.getAntCharacter();
//	totalEnergy = newTile.getTotalEnergy();
//}

Tile::Tile(Coordinate newCoordinate, AntCharacter newAntCharacter, Energy newTotalEnergy) :
		coordinate(newCoordinate.getX(), newCoordinate.getY()),
		antCharacter(newAntCharacter.getAttitude(), newAntCharacter.getTrait(), newAntCharacter.getOccupancy()) {
	coordinate = newCoordinate;
	antCharacter = newAntCharacter;
	totalEnergy = newTotalEnergy;
}

Coordinate Tile::getCoordinate() {
	return coordinate;
}

AntCharacter Tile::getAntCharacter() {
	return antCharacter;
}

Energy Tile::getTotalEnergy() {
	return totalEnergy;
}

void Tile::setAntCharacter(AntCharacter newAntCharacter) {
	antCharacter = newAntCharacter;
}

void Tile::setCoordinate(Coordinate newCoordinate) {
	coordinate = newCoordinate;
}

void Tile::setTotalEnergy(Energy newTotalEnergy) {
	totalEnergy = newTotalEnergy;
}