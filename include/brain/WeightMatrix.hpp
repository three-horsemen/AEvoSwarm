//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_WEIGHTMATRIX_HPP
#define SFTP_WEIGHTMATRIX_HPP

#include <brain/Neuron.hpp>

class WeightMatrix {
	int inputSize, outputSize;
	weight **weights;
public:
	WeightMatrix(int, int);

	~WeightMatrix();

	weight *getNeuronWeights(int);
};


#endif //SFTP_WEIGHTMATRIX_HPP
