//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_AGENTCHARACTER_HPP
#define SFTP_AGENTCHARACTER_HPP

//#include "environment/Tile.hpp"

typedef unsigned short Attitude;
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

#endif //SFTP_AGENTCHARACTER_HPP
