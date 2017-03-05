//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_ANT_HPP
#define SFTP_ANT_HPP

#include <shared/logger.hpp>

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

	bool isActionValid(Agent::Action action) {
		switch (action) {
			default:
				throw invalid_argument("Unknown action specified");
		}
	}
};


#endif //SFTP_ANT_HPP
