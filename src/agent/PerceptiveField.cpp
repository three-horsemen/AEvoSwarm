//
// Created by reuben on 6/3/17.
//

#include "agent/PerceptiveField.hpp"


PerceptiveField::PerceptiveField() {

}

Tile PerceptiveField::getTile(Coordinate coordinate) {
	return tile[coordinate.getX()][coordinate.getY()];
}
