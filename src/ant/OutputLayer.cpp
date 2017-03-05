//
// Created by reuben on 5/3/17.
//

#include "ant/OutputLayer.hpp"

OutputLayer::OutputLayer(int size)
		: Layer(OUTPUT, size, size) {
	this->inputSize = this->outputSize = size;
}

OutputLayer::~OutputLayer() {
}

void OutputLayer::compute() {
	setOutputs(getInputs());
}

void OutputLayer::displayOutputs() {
	for (int i = 0; i < outputSize; i++) {
		cout << outputs[i] << ' ';
	}
}