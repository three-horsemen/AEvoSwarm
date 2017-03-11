//
// Created by soorya on 10/3/17.
//

#ifndef AEVO_OPENCV_HPP
#define AEVO_OPENCV_HPP

#include <agent/PerceptiveField.hpp>
#include <agent/Ant.hpp>
#include <ui/OpenCVTile.hpp>
#include "environment/Tile.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
using namespace cv;

namespace ui {
	class OpenCV {
	protected:
		Mat image;
		Rect2d HUDBackdrop;
		const static unsigned short WINDOW_WIDTH = 800;
		const static unsigned short WINDOW_HEIGHT = 800;
		unsigned short TILE_SIDE_PIXEL_WIDTH;
		unsigned short TILE_SIDE_PIXEL_HEIGHT;
		unsigned short AGENT_HEIGHT_DISPLAY_PADDING;
		unsigned short AGENT_WIDTH_DISPLAY_PADDING;
		unsigned short HUD_HEIGHT_MARGIN;
		unsigned short HUD_HEIGHT_PADDING;
		const Environment &environment;
	public:
//		vector<vector<OpenCVTile> > tiles;

		OpenCV(Environment &);

		Environment getEnvironment() const;

		char displayEnvironment(const vector<Ant> &,
								unsigned long long &); //Return ASCII value of last pressed key. Else, wait for 1 millisecond.

		void displayPerceptiveField(const PerceptiveField &);
	};

}

#endif //AEVO_OPENCV_HPP
