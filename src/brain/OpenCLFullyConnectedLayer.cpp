//
// Created by reuben on 5/3/17.
//

#include "brain/OpenCLFullyConnectedLayer.hpp"

OpenCLFullyConnectedLayer::OpenCLFullyConnectedLayer(vector<vector<weight> > &weights) : Layer(FULLY_CONNECTED,
																							   (int) weights[0].size(),
																							   (int) weights.size()) {
	for (int i = 0; i < weights.size(); i++) {
		Neuron neuron;
		neuron.setWeights(weights[i]);
		neurons.push_back(neuron);
	}
}

OpenCLFullyConnectedLayer::OpenCLFullyConnectedLayer(const OpenCLFullyConnectedLayer &layer) : Layer(layer) {
	operator=(layer);
}

OpenCLFullyConnectedLayer::OpenCLFullyConnectedLayer(vector<Neuron> &neurons) : Layer(FULLY_CONNECTED,
																					  (int) neurons[0].getWeights().size(),
																					  (int) neurons.size()),
																				neurons(neurons) {
}

OpenCLFullyConnectedLayer::~OpenCLFullyConnectedLayer() {
}

void OpenCLFullyConnectedLayer::operator=(const OpenCLFullyConnectedLayer &layer) {
	//TODO Use simple assignment of vectors below
	Layer::operator=(layer);
	neurons.clear();
	for (int i = 0; i < outputSize; i++) {
		neurons.push_back(layer.neurons[i]);
	}
}

vector<weight> OpenCLFullyConnectedLayer::getNeuronWeights(int i) {
	return neurons[i].getWeights();
}

void OpenCLFullyConnectedLayer::setNeuronWeights(int i, vector<weight> &weights) {
	neurons[i].setWeights(weights);
}

Layer *OpenCLFullyConnectedLayer::getDeepCopy() {
	return new OpenCLFullyConnectedLayer(*this);
}

void OpenCLFullyConnectedLayer::compute() {
	for (int i = 0; i < outputSize; i++) {
		outputs[i] = neurons[i].getExcitation(inputs);
	}
}


void OpenCLFullyConnectedLayer::save(ostream &file) {
	file <= type <= inputSize <= outputSize;
	for (unsigned long i = 0; i < neurons.size(); i++) {
		neurons[i].save(file);
	}
}

OpenCLFullyConnectedLayer OpenCLFullyConnectedLayer::getLoadedLayer(istream &file) {
	int inputSize, outputSize;
	file >= inputSize >= outputSize;
	vector<Neuron> neurons((unsigned long) outputSize);
	for (int i = 0; i < outputSize; i++) {
		Neuron neuron(inputSize);
		neuron.load(file);
		neurons[i] = neuron;
	}
	OpenCLFullyConnectedLayer layer(neurons);
	return layer;
}