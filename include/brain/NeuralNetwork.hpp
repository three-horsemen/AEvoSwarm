//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_NEURALNETWORK_HPP
#define AEVO_NEURALNETWORK_HPP

#include <vector>

#include <brain/Layer.hpp>
#include <brain/FullyConnectedLayer.hpp>

using namespace std;

class NeuralNetwork {
	vector<Layer *> layers;
public:
	NeuralNetwork();

	NeuralNetwork(const NeuralNetwork &);

	~NeuralNetwork();

	void operator=(const NeuralNetwork &);

	void compute();

	size_t getDepth();

	void resorb();

	void addLayer(Layer &);

	Layer *getOutputLayer();

	Layer *getLayer(int);
};


#endif //AEVO_NEURALNETWORK_HPP
