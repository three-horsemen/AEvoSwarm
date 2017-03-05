//
// Created by reuben on 5/3/17.
//

#include <brain/NeuralNetwork.hpp>

NeuralNetwork::NeuralNetwork() {
}

NeuralNetwork::~NeuralNetwork() {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->free();
	}
	layers.clear();
}

void NeuralNetwork::compute() {
	if (layers.back()->type != OUTPUT) {
		throw runtime_error("The final layer of the network must be an output layer");
	}
	layers[0]->compute();
	excitation *excitations = layers[0]->getOutputs();
	for (int i = 1; i < layers.size(); i++) {
		Layer *layer = layers[i];
		layer->setInputs(excitations);
		layer->compute();
		excitations = layer->getOutputs();
	}
}

void NeuralNetwork::addLayer(Layer &layer) {
	if (layers.size() == 0) {
		if (layer.type == INPUT) {
			layers.push_back(&layer);
		} else {
			throw invalid_argument("First layer must be input layer");
		}
	} else {
		if (layers.back()->outputSize == layer.inputSize) {
			layers.push_back(&layer);
		} else {
			string errStr =
					"Layer input size does not match output size of layer index " + to_string(layers.size() - 1);
			throw invalid_argument(errStr);
		}
	}
}

Layer *NeuralNetwork::getLayer(int level) {
	if (level < layers.size()) {
		return layers[level];
	} else {
		throw invalid_argument("Cannot get layer beyond the network depth");
	}
}

Layer *NeuralNetwork::getOutputLayer() {
	return layers.back();
}