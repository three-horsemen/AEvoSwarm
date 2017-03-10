//
// Created by soorya on 10/3/17.
//

#include "ui/OpenCVTile.hpp"

void OpenCVTile::drawOpenCVTile(Mat image) {
	rectangle(
			image,
			Point(topLeft.getX(), topLeft.getY()),
			Point(bottomRight.getX(), bottomRight.getY()),
			Scalar(110, 220, 0),
//			CV_FILLED,
			1,
			8
	);

}