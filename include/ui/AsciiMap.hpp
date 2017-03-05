//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_ASCIIMAP_HPP
#define SFTP_ASCIIMAP_HPP

#include "map/Map.hpp"

class AsciiMap {
protected:
	Map map;
	const char charOccupancy[5] = {' ', '^', 'v', '>', '<'}; //Dead, North, South, East, West
public:
	AsciiMap(Map &);

	Map getMap();

	void setMap(Map);

	void displayAsciiMap();
};

#endif //SFTP_ASCIIMAP_HPP
