//
// Created by reuben on 5/3/17.
//

#include "brain/Layer.hpp"

Layer::Layer(LayerType type, unsigned int inputSize, int outputSize) :
		inputs((unsigned long) inputSize),
		outputs((unsigned long) outputSize) {
	this->type = type;
	this->inputSize = inputSize;
	this->outputSize = outputSize;
}

Layer::Layer(const Layer &layer) {
	operator=(layer);
}

Layer::~Layer() {
}

void Layer::operator=(const Layer &layer) {
	this->type = layer.type;
	this->inputSize = layer.inputSize;
	this->outputSize = layer.outputSize;
	inputs.clear();
	for (int i = 0; i < inputSize; i++) {
		inputs.push_back(layer.inputs[i]);
	}
	outputs.clear();
	for (int i = 0; i < outputSize; i++) {
		outputs.push_back(layer.outputs[i]);
	}
}

void Layer::setInputs(const vector<excitation> &excitations) {
	for (int i = 0; i < inputSize; i++) {
		inputs[i] = excitations[i];
	}
}

void Layer::setOutputs(const vector<excitation> &excitations) {
	for (int i = 0; i < outputSize; i++) {
		outputs[i] = excitations[i];
	}
}

vector<excitation> Layer::getInputs() {
	return inputs;
}

vector<excitation> Layer::getOutputs() {
	return outputs;
}

void Layer::displayOutputs() {
	for (int i = 0; i < outputSize; i++) {
		cout << outputs[i] << ' ';
	}
}

void Layer::compute() {
	throw runtime_error("Layer not implemented");
}