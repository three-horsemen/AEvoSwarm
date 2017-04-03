//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_OPENCLFULLYCONNECTEDLAYER_HPP
#define AEVO_OPENCLFULLYCONNECTEDLAYER_HPP

#include <brain/Layer.hpp>
#include <brain/Neuron.hpp>

#include <shared/Utils.hpp>
#include <opencl/Utils.hpp>

#include <CL/cl.h>

class OpenCLFullyConnectedLayer : public Layer {
	static cl_context context;
	static cl_kernel kernel;
	static cl_command_queue cmdQ;
protected:
	cl_mem inputsD, weightsD, outputsD;
public:
	static void staticInit();

	static void staticDeinit();

	void init(unsigned int, unsigned int);

	void deinit();

	OpenCLFullyConnectedLayer(vector<vector<weight> > &);

	OpenCLFullyConnectedLayer(unsigned int, unsigned int, weight *);

	OpenCLFullyConnectedLayer(const OpenCLFullyConnectedLayer &);

	~OpenCLFullyConnectedLayer();

	void operator=(const OpenCLFullyConnectedLayer &);

	vector<vector<weight>> getNeuronWeights();

	void setNeuronWeights(vector<vector<weight> > &);

	void setNeuronWeights(unsigned int, unsigned int, weight *);

	virtual void setInputs(const vector<excitation> &);

	virtual void setOutputs(const vector<excitation> &);

	Layer *getDeepCopy();

	void compute();

	void save(ostream &);

	static OpenCLFullyConnectedLayer getLoadedLayer(istream &);
};


#endif //AEVO_OPENCLFULLYCONNECTEDLAYER_HPP
