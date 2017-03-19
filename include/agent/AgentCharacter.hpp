//
// Created by soorya on 5/3/17.
//

#ifndef AEVO_AGENTCHARACTER_HPP
#define AEVO_AGENTCHARACTER_HPP

//#include "environment/Tile.hpp"

typedef unsigned short Attitude;
namespace attitude {
	const Attitude MAX_ATTITUDE = -1;
};

typedef unsigned short Trait;
typedef enum {
	OCCUPANCY_DEAD = 0,
	OCCUPANCY_NORTH = 2,
	OCCUPANCY_SOUTH = 3,
	OCCUPANCY_EAST = 4,
	OCCUPANCY_WEST = 5
} Occupancy;

class AgentCharacter {
protected:
	Attitude attitude;
	Trait trait;
	Occupancy occupancy;
public:
	AgentCharacter();

	AgentCharacter(Attitude newAttitude, Trait newTrait, Occupancy newOccupancy);

	Attitude getAttitude() const;

	Trait getTrait() const;

	Occupancy getOccupancy() const;

	void setAttitude(Attitude);

	void setTrait(Trait);

	void setOccupancy(Occupancy);
};

#endif //AEVO_AGENTCHARACTER_HPP
