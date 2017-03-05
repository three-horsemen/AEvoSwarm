//
// Created by soorya on 5/3/17.
//

#include "shared/AntCharacter.hpp"

//AntCharacter::AntCharacter(AntCharacter &newAntCharacter) {
//	attitude = newAntCharacter.getAttitude();
//	trait = newAntCharacter.getTrait();
//	occupancy = newAntCharacter.getOccupancy();
//}

AntCharacter::AntCharacter(Attitude newAttitude, Trait newTrait, Occupancy newOccupancy) {
	attitude = newAttitude;
	trait = newTrait;
	occupancy = newOccupancy;
}

Attitude AntCharacter::getAttitude() const {
	return attitude;
}

Occupancy AntCharacter::getOccupancy() const {
	return occupancy;
}

Trait AntCharacter::getTrait() const {
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