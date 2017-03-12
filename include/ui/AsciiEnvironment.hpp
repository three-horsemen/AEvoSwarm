//
// Created by soorya on 5/3/17.
//

#ifndef AEVO_ASCIIENVIRONMENT_HPP
#define AEVO_ASCIIENVIRONMENT_HPP

#include <iostream>
#include <environment/Environment.hpp>
#include <agent/PerceptiveField.hpp>

class AsciiEnvironment {
protected:
	Environment environment;
public:
	AsciiEnvironment(Environment &);

	Environment getEnvironment() const;

	void setEnvironment(Environment);

	static void displayAsciiEnvironment(Environment &);

	static void displayEnergyMatrix(Environment &);

	static void displayEnergyDeltas(const Environment &, const Environment &);

	static void displayPerceptiveField(PerceptiveField &);
};

#endif //AEVO_ASCIIENVIRONMENT_HPP
