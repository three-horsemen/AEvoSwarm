//
// Created by reuben on 5/3/17.
//

#include "brain/Neuron.hpp"

Neuron::Neuron() {
	weights = 0;
}

Neuron::~Neuron() {
	if (weights) {
		delete[] weights;
	}
}

void Neuron::setWeights(int inputSize, weight *weights) {
	if (this->weights) {
		delete[] this->weights;

	}
	this->inputSize = inputSize;
	this->weights = new weight[inputSize];
	this->inputSize = inputSize;
	for (int i = 0; i < inputSize; i++)
		this->weights[i] = weights[i];
}

excitation Neuron::getExcitation(excitation *excitations) {
	excitation excitation = 0;
	for (int i = 0; i < inputSize; i++) {
		excitation += excitations[i] * weights[i];
	}
	return tanh(excitation);
}

namespace neuron {
	void randomizeExcitation(int size, excitation *excitations) {
		for (int i = 0; i < size; i++) {
			excitations[i] = ((float) rand()) / (RAND_MAX);
		}
	}

	void randomizeWeights(int size, weight *weights) {
		for (int i = 0; i < size; i++) {
			weights[i] = 0.085f * ((float) rand()) / (RAND_MAX);
		}
	}

	void randomizeWeights(int inputSize, int outputSize, weight *weights) {
		for (int i = 0; i < outputSize; i++) {
			randomizeWeights(inputSize, &weights[inputSize * i]);
		}
	}
}