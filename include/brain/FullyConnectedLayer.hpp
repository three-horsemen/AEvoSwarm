//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_FULLYCONNECTEDLAYER_HPP
#define SFTP_FULLYCONNECTEDLAYER_HPP

#include <brain/Layer.hpp>
#include <brain/WeightMatrix.hpp>
#include <brain/Neuron.hpp>

class FullyConnectedLayer : public Layer {
	Neuron *neurons;
public:
	FullyConnectedLayer(int, int, weight *);

	~FullyConnectedLayer();

	void compute();
};


#endif //SFTP_FULLYCONNECTEDLAYER_HPP
