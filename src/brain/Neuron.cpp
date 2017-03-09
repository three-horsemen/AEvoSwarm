//
// Created by reuben on 5/3/17.
//

#include "brain/Neuron.hpp"

Neuron::Neuron() {
}

Neuron::~Neuron() {
}

void Neuron::setWeights(vector<weight> &weights) {
	this->inputSize = (int) weights.size();

	this->weights.clear();
	for (int i = 0; i < weights.size(); i++)
		this->weights.push_back(weights[i]);
}

excitation Neuron::getExcitation(vector<excitation> excitations) {
	excitation excitation = 0;
	for (int i = 0; i < inputSize; i++) {
		excitation += excitations[i] * weights[i];
	}
	return tanh(excitation);
}

namespace neuron {
	void randomizeExcitation(vector<excitation> &excitations) {
		for (int i = 0; i < excitations.size(); i++) {
			excitations[i] = ((float) rand()) / (RAND_MAX);
		}
	}

	void randomizeWeights(vector<weight> &weights) {
		for (int i = 0; i < weights.size(); i++) {
			weights[i] = 0.085f * ((float) rand()) / (RAND_MAX);
		}
	}

	void randomizeWeights(vector<vector<weight> > &weights) {
		for (int i = 0; i < weights.size(); i++) {
			randomizeWeights(weights[i]);
		}
	}
}