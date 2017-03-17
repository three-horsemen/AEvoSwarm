//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_INPUTLAYER_HPP
#define AEVO_INPUTLAYER_HPP


#include "NeuralNetwork.hpp"
#include "Layer.hpp"

class InputLayer : public Layer {
public:
	InputLayer(int);

	InputLayer(const InputLayer &);

	~InputLayer();

	void operator=(InputLayer &);

	Layer *getDeepCopy();

	void compute();

	void save(ofstream &);

	static InputLayer getLoadedLayer(ifstream &);
};


#endif //AEVO_INPUTLAYER_HPP
