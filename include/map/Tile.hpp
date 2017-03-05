//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_TILE_HPP
#define SFTP_TILE_HPP

#include "map/Coordinate.hpp"
#include "shared/AntCharacter.hpp"
#include "map/Energy.hpp"

class Tile {
protected:
	Coordinate coordinate;
	AntCharacter antCharacter;
	Energy totalEnergy;
public:
	Tile() {}

	Tile(Coordinate, AntCharacter, Energy);

	Coordinate getCoordinate();

	AntCharacter getAntCharacter();

	Energy getTotalEnergy();

	void setCoordinate(Coordinate);

	void setAntCharacter(AntCharacter);

	void setTotalEnergy(Energy);
};


#endif //SFTP_TILE_HPP
