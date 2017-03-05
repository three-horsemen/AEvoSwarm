//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_ANT_HPP
#define SFTP_ANT_HPP

#include <shared/logger.hpp>
#include <shared/Utils.hpp>

#include <agent/Agent.hpp>

using namespace std;

class Ant : public Agent {
public:
	enum Action {
		FORWARD,
		LEFT,
		RIGHT,
		EAT,
		ATTACK,
		FORTIFY,
		MATURE,
		GROW_BABY,
		GIVE_BIRTH
	};

	Ant(Environment &);

	bool isActionValid(Agent::Action action);

	void observeEnvironment(Environment &);

	PerceptiveField *getPerceptiveField();
};


#endif //SFTP_ANT_HPP
