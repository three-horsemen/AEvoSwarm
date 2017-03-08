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

	~NeuralNetwork();

	void freeLayers();

	void compute();

	void addLayer(Layer &);

	Layer *getOutputLayer();

	Layer *getLayer(int);
};


#endif //SFTP_NEURALNETWORK_HPP
