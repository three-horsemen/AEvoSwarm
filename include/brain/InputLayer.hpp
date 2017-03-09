//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_INPUTLAYER_HPP
#define SFTP_INPUTLAYER_HPP


#include "NeuralNetwork.hpp"
#include "Layer.hpp"

class InputLayer : public Layer {
public:
	InputLayer(int);

	InputLayer(InputLayer &);

	~InputLayer();

	void set(InputLayer &);

	InputLayer operator=(InputLayer &);

	Layer *getDeepCopy();

	void compute();
};


#endif //SFTP_INPUTLAYER_HPP
