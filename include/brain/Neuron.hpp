//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_NEURON_HPP
#define AEVO_NEURON_HPP

#include <random>
#include <fstream>

#include <shared/Utils.hpp>

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

	Neuron(int);

	~Neuron();

	void setWeights(vector<weight> &);

	vector<weight> getWeights();

	excitation getExcitation(vector<excitation>);

	void save(ostream &);

	void load(istream &);

	static void randomizeExcitation(vector<excitation> &);

	static void randomizeWeights(vector<weight> &);

	static void randomizeWeights(vector<vector<weight> > &);

	static void mutateWeights(vector<weight> &, float);
};

#endif //AEVO_NEURON_HPP
