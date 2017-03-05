//
// Created by reuben on 5/3/17.
//

#include <agent/Ant.hpp>

Ant::Ant(Environment &environment) : Agent(environment) {

}

bool Ant::isActionValid(Agent::Action action) {
	switch (action) {
		default:
			throw invalid_argument("Unknown action specified");
	}
}

void Ant::observeEnvironment(Environment &environment) {
	for (int i = coordinate.getX() - PerceptiveField::WIDTH / 2;
		 i < coordinate.getX() + PerceptiveField::WIDTH / 2; i++) {
		for (int j = coordinate.getY() - PerceptiveField::HEIGHT / 2;
			 j < coordinate.getY() + PerceptiveField::HEIGHT / 2; j++) {
			int x = Utils::modulo(i, MAP_MAX_X), y = Utils::modulo(j, MAP_MAX_Y);
			Tile tile = environment.getTile(Coordinate(x, y));
			perceptiveField.tile[i + PerceptiveField::WIDTH / 2][j + PerceptiveField::HEIGHT / 2] = tile;
		}
	}
}

PerceptiveField *Ant::getPerceptiveField() {
	return &perceptiveField;
}