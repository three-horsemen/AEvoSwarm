//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_FULLYCONNECTEDLAYER_HPP
#define SFTP_FULLYCONNECTEDLAYER_HPP

#include <ant/Layer.hpp>
#include <ant/WeightMatrix.hpp>
#include <ant/Neuron.hpp>

class FullyConnectedLayer : public Layer {
	Neuron *neurons;
public:
	FullyConnectedLayer(int, int, weight *);

	~FullyConnectedLayer();

	void compute();
};


#endif //SFTP_FULLYCONNECTEDLAYER_HPP
