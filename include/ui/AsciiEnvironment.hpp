//
// Created by soorya on 5/3/17.
//

#ifndef SFTP_ASCIIENVIRONMENT_HPP
#define SFTP_ASCIIENVIRONMENT_HPP

#include "environment/Environment.hpp"
#include <agent/PerceptiveField.hpp>

class AsciiEnvironment {
protected:
	Environment environment;
public:
	AsciiEnvironment(Environment &);

	Environment getEnvironment();

	void setEnvironment(Environment);

	static void displayAsciiEnvironment(Environment &);

	static void displayPerceptiveField(PerceptiveField &);
};

#endif //SFTP_ASCIIENVIRONMENT_HPP
