//
// Created by soorya on 5/3/17.
//

#ifndef AEVO_COORDINATE_HPP
#define AEVO_COORDINATE_HPP

class Coordinate {
protected:
	int X;
	int Y;
public:
	Coordinate();

	Coordinate(int, int);

	int getX() const;

	int getY() const;

	void setX(int);

	void setY(int);

	bool operator==(Coordinate);

	bool operator!=(Coordinate);
};


#endif //AEVO_COORDINATE_HPP
