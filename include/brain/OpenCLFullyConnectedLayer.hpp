//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_FULLYCONNECTEDLAYER_HPP
#define AEVO_FULLYCONNECTEDLAYER_HPP

#include <brain/Layer.hpp>
#include <brain/Neuron.hpp>

#include <shared/Utils.hpp>

class OpenCLFullyConnectedLayer : public Layer {
protected:
	vector<Neuron> neurons;//TODO Make private
public:
	OpenCLFullyConnectedLayer(vector<vector<weight> > &);

	OpenCLFullyConnectedLayer(const OpenCLFullyConnectedLayer &);

	OpenCLFullyConnectedLayer(vector<Neuron> &);

	~OpenCLFullyConnectedLayer();

	void operator=(const OpenCLFullyConnectedLayer &);

	vector<weight> getNeuronWeights(int i);

	void setNeuronWeights(int i, vector<weight> &);

	Layer *getDeepCopy();

	void compute();

	void save(ostream &);

	static OpenCLFullyConnectedLayer getLoadedLayer(istream &);
};


#endif //AEVO_FULLYCONNECTEDLAYER_HPP
