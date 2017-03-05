//
// Created by reuben on 5/3/17.
//

#include "ant/WeightMatrix.hpp"

WeightMatrix::WeightMatrix(int inputSize, int outputSize) {
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	this->weights = new weight *[outputSize];
	for (int i = 0; i < inputSize; i++) {
		weights[i] = new weight[inputSize];
	}
}

WeightMatrix::~WeightMatrix() {
	delete[] weights;
}

weight *WeightMatrix::getNeuronWeights(int row) {
	return this->weights[row];
}