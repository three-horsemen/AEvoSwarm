//
// Created by reuben on 5/3/17.
//

#include <agent/Agent.hpp>

Agent::Agent(short perceptiveFieldWidth, short perceptiveFieldHeight) : perceptiveField(perceptiveFieldWidth,
																						perceptiveFieldHeight) {

}

Agent::Agent(const Agent &agent) :
		perceptiveField(agent.perceptiveField),
		brain(agent.brain) {
	operator=(agent);
}

void Agent::operator=(const Agent &agent) {
	perceptiveField = agent.perceptiveField;
	globalCoordinate = agent.globalCoordinate;
	brain = agent.brain;
	selectedAction = agent.selectedAction;
}

PerceptiveField *Agent::getPerceptiveField() {
	return &perceptiveField;
}
