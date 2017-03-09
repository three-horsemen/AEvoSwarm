//
// Created by reuben on 5/3/17.
//

#include <agent/Agent.hpp>

void Agent::initialize() {
	srand((unsigned int) time(NULL));
}

Agent::Agent(short perceptiveFieldWidth, short perceptiveFieldHeight) : perceptiveField(perceptiveFieldWidth,
																						perceptiveFieldHeight) {

}

Agent::Agent(Agent &agent) :
		perceptiveField(agent.perceptiveField),
		brain(agent.brain) {
	operator=(agent);
}

void Agent::operator=(Agent &agent) {
	perceptiveField = agent.perceptiveField;
	globalCoordinate = agent.globalCoordinate;
	brain = agent.brain;
	selectedAction = agent.selectedAction;
}

PerceptiveField *Agent::getPerceptiveField() {
	return &perceptiveField;
}
