//
// Created by reuben on 5/3/17.
//

#include "brain/Neuron.hpp"

Neuron::Neuron() {
}

Neuron::~Neuron() {
}

vector<weight> Neuron::getWeights() {
	return weights;
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

void Neuron::randomizeExcitation(vector<excitation> &excitations) {
	for (int i = 0; i < excitations.size(); i++) {
		excitations[i] = ((float) rand()) / (RAND_MAX);
	}
}

void Neuron::randomizeWeights(vector<weight> &weights) {
	for (int i = 0; i < weights.size(); i++) {
		weights[i] = 0.085f * ((float) rand()) / (RAND_MAX);
	}
}

void Neuron::randomizeWeights(vector<vector<weight> > &weights) {
	for (int i = 0; i < weights.size(); i++) {
		randomizeWeights(weights[i]);
	}
}

void Neuron::mutateWeights(vector<weight> &weights, float variance) {
	std::default_random_engine generator;
	normal_distribution<float> distribution(1, variance);
	for (int i = 0; i < weights.size(); i++) {
		weights[i] *= distribution(generator);
	}
}
