//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_NEURALNETWORK_HPP
#define SFTP_NEURALNETWORK_HPP

#include <vector>

#include <brain/Layer.hpp>
#include <brain/FullyConnectedLayer.hpp>

using namespace std;

class NeuralNetwork {
	vector<Layer *> layers;
public:
	NeuralNetwork();

	NeuralNetwork(NeuralNetwork &);

	~NeuralNetwork();

	void operator=(NeuralNetwork &);

	void compute();

	size_t getDepth();

	void resorb();

	void addLayer(Layer &);

	Layer *getOutputLayer();

	Layer *getLayer(int);
};


#endif //SFTP_NEURALNETWORK_HPP
