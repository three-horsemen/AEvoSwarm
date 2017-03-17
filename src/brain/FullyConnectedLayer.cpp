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

FullyConnectedLayer::FullyConnectedLayer(const FullyConnectedLayer &layer) : Layer(layer) {
	operator=(layer);
}

FullyConnectedLayer::FullyConnectedLayer(vector<Neuron> &neurons) : Layer(FULLY_CONNECTED,
																		  (int) neurons[0].getWeights().size(),
																		  (int) neurons.size()),
																	neurons(neurons) {
}

FullyConnectedLayer::~FullyConnectedLayer() {
}

void FullyConnectedLayer::operator=(const FullyConnectedLayer &layer) {
	//TODO Use simple assignment of vectors below
	Layer::operator=(layer);
	neurons.clear();
	for (int i = 0; i < outputSize; i++) {
		neurons.push_back(layer.neurons[i]);
	}
}

vector<weight> FullyConnectedLayer::getNeuronWeights(int i) {
	return neurons[i].getWeights();
}

void FullyConnectedLayer::setNeuronWeights(int i, vector<weight> &weights) {
	neurons[i].setWeights(weights);
}

Layer *FullyConnectedLayer::getDeepCopy() {
	return new FullyConnectedLayer(*this);
}

void FullyConnectedLayer::compute() {
	for (int i = 0; i < outputSize; i++) {
		outputs[i] = neurons[i].getExcitation(inputs);
	}
}


void FullyConnectedLayer::save(ofstream &file) {
	file << type << ' ' << inputSize << ' ' << outputSize << '\t';
	for (unsigned long i = 0; i < neurons.size(); i++) {
		neurons[i].save(file);
	}
	file << endl;
}

FullyConnectedLayer FullyConnectedLayer::getLoadedLayer(ifstream &file) {
	int inputSize, outputSize;
	file >> inputSize >> outputSize;
	vector<Neuron> neurons((unsigned long) outputSize);
	for (int i = 0; i < outputSize; i++) {
		Neuron neuron(inputSize);
		neuron.load(file);
		neurons[i] = neuron;
	}
	FullyConnectedLayer layer(neurons);
	return layer;
}