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
	excitation *inputs, *outputs;
public:
	int inputSize, outputSize;
	LayerType type;

	Layer(LayerType, int, int);

	void setInputs(excitation *);

	void setOutputs(excitation *);

	excitation *getInputs();

	excitation *getOutputs();

	void free();

	virtual void displayOutputs();

	virtual ~Layer();

	virtual void compute();
};


#endif //SFTP_LAYER_HPP
