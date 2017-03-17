//
// Created by reuben on 5/3/17.
//

#include "brain/OutputLayer.hpp"

OutputLayer::OutputLayer(int size)
		: Layer(OUTPUT, size, size) {
	this->inputSize = this->outputSize = size;
}

OutputLayer::OutputLayer(const OutputLayer &outputLayer)
		: Layer(outputLayer) {
}

OutputLayer::~OutputLayer() {
}

void OutputLayer::compute() {
	setOutputs(getInputs());
}

Layer *OutputLayer::getDeepCopy() {
	return new OutputLayer(*this);
}

void OutputLayer::displayOutputs() {
	for (int i = 0; i < outputSize; i++) {
		cout << outputs[i] << ' ';
	}
}

void OutputLayer::save(ofstream &file) {
	file << type << ' ' << inputSize << ' ' << outputSize << endl;
}

OutputLayer OutputLayer::getLoadedLayer(ifstream &file) {
	int inputSize, outputSize;
	file >> inputSize >> outputSize;
	OutputLayer outputLayer(inputSize);
	return outputLayer;
}