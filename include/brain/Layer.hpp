//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_LAYER_HPP
#define SFTP_LAYER_HPP

#include <iostream>

#include <brain/Neuron.hpp>

using namespace std;

enum LayerType {
	INPUT,
	OUTPUT,
	FULLY_CONNECTED
};

class Layer {
protected:
	vector<excitation> inputs;
	vector<excitation> outputs;
public:
	int inputSize, outputSize;
	LayerType type;

	Layer(LayerType, int, int);

	Layer(Layer &);

	void operator=(Layer &);

	virtual Layer *getDeepCopy()=0;

	void setInputs(vector<excitation> &);

	void setOutputs(const vector<excitation> &);

	vector<excitation> getInputs();

	vector<excitation> getOutputs();

	virtual void displayOutputs();

	virtual ~Layer();

	virtual void compute();
};


#endif //SFTP_LAYER_HPP
