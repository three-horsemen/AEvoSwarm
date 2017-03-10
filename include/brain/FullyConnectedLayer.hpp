//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_FULLYCONNECTEDLAYER_HPP
#define AEVO_FULLYCONNECTEDLAYER_HPP

#include <brain/Layer.hpp>
#include <brain/Neuron.hpp>

class FullyConnectedLayer : public Layer {
protected:
	vector<Neuron> neurons;//TODO Make private
public:
	FullyConnectedLayer(vector<vector<weight> > &);

	FullyConnectedLayer(FullyConnectedLayer &);

	~FullyConnectedLayer();

	FullyConnectedLayer operator=(FullyConnectedLayer &);

	void set(FullyConnectedLayer &);

	vector<weight> getNeuronWeights(int i);

	vector<weight> setNeuronWeights(int i, vector<weight> &);

	Layer *getDeepCopy();


	void compute();
};


#endif //AEVO_FULLYCONNECTEDLAYER_HPP
