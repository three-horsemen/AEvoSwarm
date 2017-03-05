//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_COORDINATE_HPP
#define SFTP_COORDINATE_HPP

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
};


#endif //SFTP_COORDINATE_HPP
