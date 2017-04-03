//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_LAYER_HPP
#define AEVO_LAYER_HPP

#include <iostream>
#include <fstream>

#include <brain/Neuron.hpp>

using namespace std;

enum LayerType {
	INPUT,
	OUTPUT,
	FULLY_CONNECTED,
	OPENCL_FULLY_CONNECTED
};

class Layer {
protected:
	vector<excitation> inputs;
	vector<excitation> outputs;
public:
	unsigned int inputSize, outputSize;
	LayerType type;

	Layer(LayerType, unsigned int, int);

	Layer(const Layer &);

	virtual void operator=(const Layer &);

	virtual Layer *getDeepCopy()=0;

	virtual void setInputs(const vector<excitation> &);

	virtual void setOutputs(const vector<excitation> &);

	virtual vector<excitation> getInputs();

	virtual vector<excitation> getOutputs();

	virtual void displayOutputs();

	virtual ~Layer();

	virtual void compute();

	virtual void save(ostream &)=0;
};


#endif //AEVO_LAYER_HPP
