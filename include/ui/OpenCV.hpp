//
// Created by soorya on 10/3/17.
//

#ifndef AEVO_OPENCV_HPP
#define AEVO_OPENCV_HPP

#include <agent/PerceptiveField.hpp>

#include <ui/OpenCVTile.hpp>

using namespace cv;

namespace ui {
	class OpenCV {
	protected:
		Mat image;
		const static unsigned short WINDOW_WIDTH = 800;
		const static unsigned short WINDOW_HEIGHT = 800;
		unsigned short TILE_SIDE_PIXEL_WIDTH;
		unsigned short TILE_SIDE_PIXEL_HEIGHT;
		const Environment &environment;
	public:
		vector<vector<OpenCVTile> > tiles;

		OpenCV(Environment &);

		Environment getEnvironment() const;

		void displayEnvironment();

		void displayPerceptiveField(const PerceptiveField &);
	};

}

#endif //AEVO_OPENCV_HPP
