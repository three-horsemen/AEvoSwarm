//
// Created by reuben on 5/3/17.
//

#include "brain/InputLayer.hpp"

InputLayer::InputLayer(int size)
		: Layer(INPUT, size, size) {
	this->inputSize = this->outputSize = size;
}

InputLayer::~InputLayer() {
}

void InputLayer::compute() {
	setOutputs(getInputs());
}