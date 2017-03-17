//
// Created by reuben on 5/3/17.
//

#include "brain/InputLayer.hpp"

InputLayer::InputLayer(int size)
		: Layer(INPUT, size, size) {
	this->inputSize = this->outputSize = size;
}

InputLayer::InputLayer(const InputLayer &inputLayer)
		: Layer(inputLayer) {
}

InputLayer::~InputLayer() {
}

void InputLayer::operator=(InputLayer &inputLayer) {
	Layer::operator=(inputLayer);
}

Layer *InputLayer::getDeepCopy() {
	return new InputLayer(*this);
}

void InputLayer::compute() {
	setOutputs(getInputs());
}

void InputLayer::save(ostream &file) {
	file << type << ' ' << inputSize << ' ' << outputSize << endl;
}

InputLayer InputLayer::getLoadedLayer(istream &file) {
	int inputSize, outputSize;
	file >> inputSize >> outputSize;
	InputLayer inputLayer(inputSize);
	return inputLayer;
}