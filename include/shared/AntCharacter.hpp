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
	AntCharacter() {}

//	AntCharacter(AntCharacter &);

	AntCharacter(Attitude newAttitude, Trait newTrait, Occupancy newOccupancy);

	Attitude getAttitude() const;

	Trait getTrait() const;

	Occupancy getOccupancy() const;

	void setAttitude(Attitude);

	void setTrait(Trait);

	void setOccupancy(Occupancy);
};

#endif //SFTP_ANTCHARACTER_HPP
