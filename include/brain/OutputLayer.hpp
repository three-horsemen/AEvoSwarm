//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_OUTPUTLAYER_HPP
#define AEVO_OUTPUTLAYER_HPP

#include <brain/Layer.hpp>

#include <iostream>
#include <shared/Utils.hpp>

using namespace std;

class OutputLayer : public Layer {
public:
	OutputLayer(int);

	OutputLayer(const OutputLayer &);

	~OutputLayer();

	void compute();

	Layer *getDeepCopy();

	void displayOutputs();

	void save(ostream &);

	static OutputLayer getLoadedLayer(istream &);
};


#endif //AEVO_OUTPUTLAYER_HPP
