//
// Created by soorya on 10/3/17.
//

#include <ui/OpenCV.hpp>
#include <agent/Ant.hpp>

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

ui::OpenCV::OpenCV(Environment &newEnvironment) : environment(newEnvironment) {
	TILE_SIDE_PIXEL_WIDTH = WINDOW_WIDTH / environment.width;
	TILE_SIDE_PIXEL_HEIGHT = WINDOW_HEIGHT / environment.height;
	image = Mat::zeros(WINDOW_HEIGHT + 2 * TILE_SIDE_PIXEL_WIDTH, WINDOW_WIDTH + 2 * TILE_SIDE_PIXEL_HEIGHT, CV_8UC3);
}

Environment ui::OpenCV::getEnvironment() const {
	return environment;
}

void ui::OpenCV::displayEnvironment() {
	int i = 0, j = 0;
	Rect2d tileRect(0, 0, TILE_SIDE_PIXEL_WIDTH, TILE_SIDE_PIXEL_HEIGHT);
	Coordinate tileCoordinate;
	AgentCharacter character;
	Tile tile;
	for (i = 0; i < environment.height; i++) {
		for (j = 0; j < environment.width; j++) {
			tile = environment.getTile(Coordinate(j, i));
			character = tile.getAgentCharacter();
			tileCoordinate = tile.getGlobalCoordinate();
			tileRect.x = (1 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH;
			tileRect.y = (1 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT;
			rectangle(
					image,
					tileRect,
					Scalar(character.getTrait(), (tile.getTotalEnergy() / Ant::HYPOTHETICAL_MAX_ENERGY),
						   character.getAttitude()),
//					CV
					1,
					8
			);
		}
	}
//	cout << "| " << opencv_environment::getCharForOccupancy(
//			environment.getTile(Coordinate(j, i)).getAgentCharacter().getOccupancy()) << " ";
	cvtColor(image, image, CV_HSV2BGR_FULL);
	imshow("OpenCV-Environment", image);
//	waitKey();
}
