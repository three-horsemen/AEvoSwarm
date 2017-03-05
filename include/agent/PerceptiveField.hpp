//
// Created by reuben on 6/3/17.
//

#ifndef SFTP_PERCEPTIVEFIELD_HPP
#define SFTP_PERCEPTIVEFIELD_HPP


#include <map/Tile.hpp>

class PerceptiveField {
public:
	static const short WIDTH = 5, HEIGHT = 5;
	Tile tile[WIDTH][HEIGHT];

	PerceptiveField();

	Tile getTile(Coordinate);
};


#endif //SFTP_PERCEPTIVEFIELD_HPP
