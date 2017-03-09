//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_NEURON_HPP
#define AEVO_NEURON_HPP

#include <random>

using namespace std;

typedef float weight;
typedef float excitation;
//TODO Convert "excitation" to "Excitation" convention
//TODO Convert "weight" to "Weight" convention

class Neuron {
	int inputSize;    //TODO Not needed because VECTOR
	vector<weight> weights;
public:
	Neuron();

	~Neuron();

	void setWeights(vector<weight> &);

	excitation getExcitation(vector<excitation>);
};

namespace neuron {
	void randomizeExcitation(vector<excitation> &);

	void randomizeWeights(vector<weight> &);

	void randomizeWeights(vector<vector<weight> > &);
};

#endif //AEVO_NEURON_HPP
