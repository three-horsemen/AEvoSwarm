//
// Created by reuben on 5/3/17.
//

#include <agent/Agent.hpp>

void Agent::initialize() {
	srand(time(NULL));
}

Agent::Agent(short perceptiveFieldWidth, short perceptiveFieldHeight) : perceptiveField(perceptiveFieldWidth,
																						perceptiveFieldHeight) {

}

PerceptiveField *Agent::getPerceptiveField() {
	return &perceptiveField;
}
