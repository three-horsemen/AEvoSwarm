//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_AGENT_HPP
#define AEVO_AGENT_HPP

#include <environment/Environment.hpp>
#include <agent/PerceptiveField.hpp>
#include <brain/NeuralNetwork.hpp>

#include <brain/NeuralNetwork.hpp>

class Agent {
public:
	enum Action {
	};
protected:
	PerceptiveField perceptiveField;
	Coordinate globalCoordinate;
	NeuralNetwork brain;
	Action selectedAction;
public:

	Agent(short, short);

	Agent(const Agent &);

	void operator=(const Agent &);

	virtual bool isEnergyAvailable(Agent::Action action)=0;

	virtual bool isActionValid(Action action)=0;

	virtual void observeEnvironment(Environment &)=0;

	virtual void senseObservation(Environment &)=0;

	virtual void developBrain()=0;

	virtual void resorbBrain()=0;

	virtual void selectAction()=0;

	virtual Action getSelectedAction() const =0;

	virtual void setSelectedAction(Action, bool = false)=0; //Boolean parameter to override DIE if true.

	virtual void performAction(Agent::Action)=0;

	//virtual void affectEnvironment(Environment &)=0;

	inline Energy getActionCost(Action);

	virtual void save(ostream &)=0;

	virtual void load(istream &, Environment &)=0;

	PerceptiveField *getPerceptiveField();
};


#endif //AEVO_AGENT_HPP
