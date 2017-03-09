//
// Created by reuben on 5/3/17.
//

#include "brain/InputLayer.hpp"

InputLayer::InputLayer(int size)
		: Layer(INPUT, size, size) {
	this->inputSize = this->outputSize = size;
}

InputLayer::InputLayer(InputLayer &inputLayer)
		: Layer(INPUT, inputLayer.inputSize, inputLayer.outputSize) {
	set(inputLayer);
}

InputLayer::~InputLayer() {
}

void InputLayer::set(InputLayer &inputLayer) {
	Layer::operator=(inputLayer);
}

InputLayer InputLayer::operator=(InputLayer &inputLayer) {
	set(inputLayer);
	return *this;
}

Layer *InputLayer::getDeepCopy() {
	return new InputLayer(*this);
}

void InputLayer::compute() {
	setOutputs(getInputs());
}