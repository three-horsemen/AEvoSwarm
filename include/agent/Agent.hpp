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

	Agent(Environment);

	virtual bool isActionValid(Action action)=0;

	virtual void observeEnvironment(Environment &)=0;

	virtual PerceptiveField *getPerceptiveField()=0;
};


#endif //SFTP_AGENT_HPP