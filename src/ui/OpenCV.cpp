//
// Created by soorya on 10/3/17.
//

#include <ui/OpenCV.hpp>


namespace opencv_environment {
	char getCharForOccupancy(Occupancy occupancy) {
		if (occupancy == OCCUPANCY_DEAD)
			return ' ';
		else if (occupancy == OCCUPANCY_NORTH)
			return '^';
		else if (occupancy == OCCUPANCY_SOUTH)
			return 'v';
		else if (occupancy == OCCUPANCY_EAST)
			return '>';
		else //if (occupancy == OCCUPANCY_WEST)
			return '<';
	}
}

ui::OpenCV::OpenCV(Environment &newEnvironment) : environment(newEnvironment.width,
															  newEnvironment.height) {
	TILE_SIDE_PIXEL_WIDTH = WINDOW_WIDTH / environment.width;
	TILE_SIDE_PIXEL_HEIGHT = WINDOW_HEIGHT / environment.height;
	image = Mat::zeros(WINDOW_HEIGHT + 2 * TILE_SIDE_PIXEL_WIDTH, WINDOW_WIDTH + 2 * TILE_SIDE_PIXEL_HEIGHT, CV_8UC3);


	for (int i = 0; i < tiles.size(); i++)
		tiles[i].clear();
	tiles.clear();
	vector<OpenCVTile> row;
	for (int i = 0; i < newEnvironment.width; i++) {
		row.clear();
		for (int j = 0; j < newEnvironment.height; j++) {
			environment.setTile(
					Tile(newEnvironment.getTile(Coordinate(i, j))),
					Coordinate(i, j));
			OpenCVTile tile;
			tile.topLeft = Coordinate(TILE_SIDE_PIXEL_WIDTH * (j + 1), TILE_SIDE_PIXEL_HEIGHT * (i + 1));
			tile.topRight = Coordinate(TILE_SIDE_PIXEL_WIDTH * (j + 1), TILE_SIDE_PIXEL_HEIGHT * (i + 2));
			tile.bottomLeft = Coordinate(TILE_SIDE_PIXEL_WIDTH * (j + 2), TILE_SIDE_PIXEL_HEIGHT * (i + 1));
			tile.bottomRight = Coordinate(TILE_SIDE_PIXEL_WIDTH * (j + 2), TILE_SIDE_PIXEL_HEIGHT * (i + 2));
			row.push_back(tile);
		}
		tiles.push_back(row);
	}
}

Environment ui::OpenCV::getEnvironment() const {
	return environment;
}

void ui::OpenCV::setEnvironment(const Environment &newEnvironment) {
	environment = newEnvironment;
}

void ui::OpenCV::displayEnvironment() {
	int i = 0, j = 0;
	for (i = 0; i < environment.height; i++) {
		for (j = 0; j < environment.width; j++) {
			tiles[i][j].drawOpenCVTile(image);
		}
	}

//	cout << "| " << opencv_environment::getCharForOccupancy(
//			environment.getTile(Coordinate(j, i)).getAgentCharacter().getOccupancy()) << " ";

	imshow("OpenCV-Environment", image);
	waitKey();

}
