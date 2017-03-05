//
// Created by soorya on 5/3/17.
//

#include "agent/AgentCharacter.hpp"

//AgentCharacter::AgentCharacter(AgentCharacter &newAgentCharacter) {
//	attitude = newAgentCharacter.getAttitude();
//	trait = newAgentCharacter.getTrait();
//	occupancy = newAgentCharacter.getOccupancy();
//}

AgentCharacter::AgentCharacter(Attitude newAttitude, Trait newTrait, Occupancy newOccupancy) {
	attitude = newAttitude;
	trait = newTrait;
	occupancy = newOccupancy;
}

Attitude AgentCharacter::getAttitude() const {
	return attitude;
}

Occupancy AgentCharacter::getOccupancy() const {
	return occupancy;
}

Trait AgentCharacter::getTrait() const {
	return trait;
}

void AgentCharacter::setAttitude(Attitude newAttitude) {
	attitude = newAttitude;
}

void AgentCharacter::setOccupancy(Occupancy newOccupancy) {
	occupancy = newOccupancy;
}

void AgentCharacter::setTrait(Trait newTrait) {
	trait = newTrait;
}