//
// Created by reuben on 5/3/17.
//

#include "ant/Layer.hpp"
#include <shared/logger.hpp>

Layer::Layer(LayerType type, int inputSize, int outputSize) {
	this->type = type;
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	inputs = new excitation[inputSize];
	outputs = new excitation[outputSize];
}

Layer::~Layer() {
}

void Layer::free() {
	delete[] inputs;
	delete[] outputs;
}

void Layer::displayOutputs() {
	for (int i = 0; i < outputSize; i++) {
		cout << outputs[i] << ' ';
	}
}

void Layer::setInputs(excitation *excitations) {
	for (int i = 0; i < inputSize; i++) {
		inputs[i] = excitations[i];
	}
}

void Layer::setOutputs(excitation *excitations) {
	for (int i = 0; i < outputSize; i++) {
		outputs[i] = excitations[i];
	}
}

excitation *Layer::getInputs() {
	return inputs;
}

excitation *Layer::getOutputs() {
	return outputs;
}

void Layer::compute() {
	LOG_ERROR << "Layer::compute";
}