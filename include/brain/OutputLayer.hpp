//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_OUTPUTLAYER_HPP
#define SFTP_OUTPUTLAYER_HPP

#include <iostream>
#include <brain/Layer.hpp>

using namespace std;

class OutputLayer : public Layer {
public:
	OutputLayer(int);

	~OutputLayer();

	void compute();

	void displayOutputs();
};


#endif //SFTP_OUTPUTLAYER_HPP
