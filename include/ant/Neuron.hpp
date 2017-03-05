//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_NEURON_HPP
#define SFTP_NEURON_HPP

#include <random>

using namespace std;

typedef float weight;
typedef float excitation;

class Neuron {
	int inputSize;
	weight *weights;
public:
	Neuron();

	~Neuron();

	void setWeights(int, weight *);

	excitation getExcitation(excitation[]);
};

namespace neuron {
	void randomizeExcitation(int, excitation *);

	void randomizeWeights(int, weight *);

	void randomizeWeights(int, int, weight *);
};

#endif //SFTP_NEURON_HPP
