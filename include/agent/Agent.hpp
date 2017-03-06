//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_AGENT_HPP
#define SFTP_AGENT_HPP

#include <map/Map.hpp>
#include <agent/PerceptiveField.hpp>

enum Action {

};

class Agent {
protected:
	PerceptiveField perceptiveField;
	Coordinate coordinate;
public:
	enum Action {
	};

	Agent();

	virtual bool isEnergyAvailable(Agent::Action action)=0;

	virtual bool isActionValid(Action action)=0;

	virtual void observeEnvironment(Environment &)=0;

	PerceptiveField *getPerceptiveField();
};


#endif //SFTP_AGENT_HPP
