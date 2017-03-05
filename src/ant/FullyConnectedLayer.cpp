//
// Created by reuben on 5/3/17.
//

#include "ant/FullyConnectedLayer.hpp"

FullyConnectedLayer::FullyConnectedLayer(int inputSize, int outputSize, weight *weights) : Layer(FULLY_CONNECTED,
																								 inputSize,
																								 outputSize) {
	neurons = new Neuron[outputSize];
	for (int i = 0; i < outputSize; i++) {
		neurons[i].setWeights(inputSize, &weights[inputSize * i]);
	}
}

FullyConnectedLayer::~FullyConnectedLayer() {
	delete[] neurons;
}

void FullyConnectedLayer::compute() {
	for (int i = 0; i < outputSize; i++) {
		outputs[i] = neurons[i].getExcitation(inputs);
	}
}