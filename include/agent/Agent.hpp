//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_AGENT_HPP
#define SFTP_AGENT_HPP

#include <environment/Environment.hpp>
#include <agent/PerceptiveField.hpp>
#include <brain/NeuralNetwork.hpp>

#include <brain/NeuralNetwork.hpp>

class Agent {
protected:
	PerceptiveField perceptiveField;
	Coordinate coordinate;
	NeuralNetwork brain;
public:
	enum Action {
	};

	Agent(short, short);

	virtual bool isEnergyAvailable(Agent::Action action)=0;

	virtual bool isActionValid(Action action)=0;

	virtual void observeEnvironment(Environment &)=0;

	virtual void developBrain()=0;

	virtual Action getSelectedAction()=0;

	virtual Action performAction(Agent::Action)=0;

	virtual void affectEnvironment(Environment &)=0;

	PerceptiveField *getPerceptiveField();
};


#endif //SFTP_AGENT_HPP
