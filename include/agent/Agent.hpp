//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_AGENT_HPP
#define SFTP_AGENT_HPP

enum Action {

};

class Agent {
public:
	enum Action {
	};

	virtual bool isActionValid(Action action)=0;
};


#endif //SFTP_AGENT_HPP
