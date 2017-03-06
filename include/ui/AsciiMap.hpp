//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_ASCIIMAP_HPP
#define SFTP_ASCIIMAP_HPP

#include "map/Map.hpp"
#include <agent/PerceptiveField.hpp>

class AsciiMap {
protected:
	Map map;
public:
	AsciiMap(Map &);

	Map getMap();

	void setMap(Map);

	static void displayAsciiMap(Map &);

	static void displayPerceptiveField(PerceptiveField &);
};

#endif //SFTP_ASCIIMAP_HPP
