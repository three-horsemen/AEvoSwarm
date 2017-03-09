//
// Created by reuben on 5/3/17.
//

#include "brain/FullyConnectedLayer.hpp"

FullyConnectedLayer::FullyConnectedLayer(vector<vector<weight> > &weights) : Layer(FULLY_CONNECTED,
																				   (int) weights[0].size(),
																				   (int) weights.size()) {
	for (int i = 0; i < weights.size(); i++) {
		Neuron neuron;
		neuron.setWeights(weights[i]);
		neurons.push_back(neuron);
	}
}

FullyConnectedLayer::FullyConnectedLayer(FullyConnectedLayer &layer) : Layer(layer) {
	set(layer);
}

FullyConnectedLayer::~FullyConnectedLayer() {
}

void FullyConnectedLayer::set(FullyConnectedLayer &layer) {
	Layer::operator=(layer);
	neurons.clear();
	for (int i = 0; i < outputSize; i++) {
		neurons.push_back(layer.neurons[i]);
	}
}

FullyConnectedLayer FullyConnectedLayer::operator=(FullyConnectedLayer &layer) {
	//TODO Use simple assignment of vectors below
	set(layer);
}

Layer *FullyConnectedLayer::getDeepCopy() {
	return new FullyConnectedLayer(*this);
}

void FullyConnectedLayer::compute() {
	for (int i = 0; i < outputSize; i++) {
		outputs[i] = neurons[i].getExcitation(inputs);
	}
}