//
// Created by soorya on 5/3/17.
//

#include "shared/AntCharacter.hpp"

Attitude AntCharacter::getAttitude() {
	return attitude;
}

Occupancy AntCharacter::getOccupancy() {
	return occupancy;
}

Trait AntCharacter::getTrait() {
	return trait;
}

void AntCharacter::setAttitude(Attitude newAttitude) {
	attitude = newAttitude;
}

void AntCharacter::setOccupancy(Occupancy newOccupancy) {
	occupancy = newOccupancy;
}

void AntCharacter::setTrait(Trait newTrait) {
	trait = newTrait;
}