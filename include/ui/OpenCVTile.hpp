//
// Created by soorya on 10/3/17.
//

#ifndef AEVO_OPENCV_TILE_HPP
#define AEVO_OPENCV_TILE_HPP

#include "environment/Tile.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class OpenCVTile {
public:
	Coordinate topLeft;
	Coordinate topRight;
	Coordinate bottomLeft;
	Coordinate bottomRight;

	void drawOpenCVTile(Mat image);
};


#endif //AEVO_OPENCV_TILE_HPP
