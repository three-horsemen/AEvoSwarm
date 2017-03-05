//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_AGENTCHARACTER_HPP
#define SFTP_AGENTCHARACTER_HPP

typedef short Attitude;
typedef short Trait;
typedef enum {
	OCCUPANCY_DEAD,
	OCCUPANCY_NORTH,
	OCCUPANCY_SOUTH,
	OCCUPANCY_EAST,
	OCCUPANCY_WEST
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
