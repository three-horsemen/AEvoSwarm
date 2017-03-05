//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_ANTCHARACTER_HPP
#define SFTP_ANTCHARACTER_HPP

typedef short Attitude;
typedef short Trait;
typedef enum {
	OCCUPANCY_DEAD,
	OCCUPANCY_NORTH,
	OCCUPANCY_SOUTH,
	OCCUPANCY_EAST,
	OCCUPANCY_WEST
} Occupancy;

class AntCharacter {
protected:
	Attitude attitude;
	Trait trait;
	Occupancy occupancy;
public:
	Attitude getAttitude();
	Trait getTrait();
	Occupancy getOccupancy();
	void setAttitude(Attitude newAttitude);
	void setTrait(Trait newTrait);
	void setOccupancy(Occupancy newOccupancy);
};

#endif //SFTP_ANTCHARACTER_HPP
