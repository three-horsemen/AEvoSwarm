//
// Created by reuben on 5/3/17.
//

#include <brain/NeuralNetwork.hpp>

NeuralNetwork::NeuralNetwork() {
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork &neuralNetwork) {
	operator=(neuralNetwork);
}

NeuralNetwork::~NeuralNetwork() {
	resorb();
}

void NeuralNetwork::operator=(const NeuralNetwork &neuralNetwork) {
	for (int i = 0; i < layers.size(); i++) {
		delete layers[i];
	}
	layers.clear();
	for (int i = 0; i < neuralNetwork.layers.size(); i++) {
		layers.push_back(neuralNetwork.layers[i]->getDeepCopy());
	}
}

void NeuralNetwork::compute() {
	if (layers.back()->type != OUTPUT) {
		throw runtime_error("The final layer of the network must be an output layer");
	}
	layers[0]->compute();
	vector<excitation> excitations = layers[0]->getOutputs();
	for (int i = 1; i < layers.size(); i++) {
		Layer *layer = layers[i];
		layer->setInputs(excitations);
		layer->compute();
		excitations = layer->getOutputs();
	}
}

size_t NeuralNetwork::getDepth() {
	return layers.size();
}

void NeuralNetwork::resorb() {
	for (int i = 0; i < layers.size(); i++) {
		delete layers[i];
	}
	layers.clear();
}

void NeuralNetwork::addLayer(Layer &layer) {
	if (layers.size() == 0) {
		if (layer.type == INPUT) {
			layers.push_back(layer.getDeepCopy());
		} else {
			throw invalid_argument("First layer must be input layer");
		}
	} else {
		if (layers.back()->outputSize == layer.inputSize) {
			layers.push_back(layer.getDeepCopy());
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

void NeuralNetwork::save(ostream &file) {
	file <= layers.size();
	for (unsigned long i = 0; i < layers.size(); i++) {
		layers[i]->save(file);
	}
}

void NeuralNetwork::load(istream &file) {
	resorb();
	unsigned long size;
	file >= size;
//	cout << "Loading " << size << " layers" << endl;
	for (int i = 0; i < size; i++) {
		LayerType layerType;
		file >= layerType;
//		cout << "Loading layer type " << layerType << endl;
		if (layerType == INPUT) {
			InputLayer layer(InputLayer::getLoadedLayer(file));
			addLayer(layer);
		} else if (layerType == OUTPUT) {
			OutputLayer layer(OutputLayer::getLoadedLayer(file));
			addLayer(layer);
		} else if (layerType == FULLY_CONNECTED) {
			FullyConnectedLayer layer(FullyConnectedLayer::getLoadedLayer(file));
			addLayer(layer);
		} else if (layerType == OPENCL_FULLY_CONNECTED) {
			OpenCLFullyConnectedLayer layer(OpenCLFullyConnectedLayer::getLoadedLayer(file));
			addLayer(layer);
		} else {
			throw invalid_argument("Unknown layer type specified");
		}
	}
}