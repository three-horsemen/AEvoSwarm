//
// Created by reuben on 5/3/17.
//

#include "brain/OpenCLFullyConnectedLayer.hpp"

using namespace std;

cl_context OpenCLFullyConnectedLayer::context;
cl_kernel OpenCLFullyConnectedLayer::kernel;
cl_command_queue OpenCLFullyConnectedLayer::cmdQ;

void OpenCLFullyConnectedLayer::staticInit() {
	FILE *fp;
	fp = fopen("../AEvoSwarm/src/brain/clFullyConnectedLayer.cl", "r");
	char *src = new char[10000];

	size_t srcSize = fread(src, 1, 10000, fp);
	cout << src << endl;

	cl_int ret;

	cl_platform_id platformID;
	ret = clGetPlatformIDs(1, &platformID, NULL);
	handleError();

	cl_device_id deviceID;
	ret = clGetDeviceIDs(platformID, CL_DEVICE_TYPE_ALL, 1, &deviceID, NULL);
	handleError();

	context = clCreateContext(NULL, 1, &deviceID, NULL, NULL, &ret);
	handleError();

	cmdQ = clCreateCommandQueue(context, deviceID, NULL, &ret);

	const char *srcs[1] = {src};
	const size_t srcSizes[1] = {srcSize};
	cl_program program = clCreateProgramWithSource(context, 1, srcs, srcSizes, &ret);
	handleError();
	delete src;;
	ret = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);
	handleError();
	kernel = clCreateKernel(program, "compute", &ret);
	handleError();
}

void OpenCLFullyConnectedLayer::staticDeinit() {
	cl_int ret;

	ret = clReleaseKernel(kernel);
	handleError();
	ret = clReleaseCommandQueue(cmdQ);
	handleError();
	ret = clReleaseContext(context);
	handleError();
}

void OpenCLFullyConnectedLayer::init(unsigned int outputSize, unsigned int inputSize) {
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	cl_int ret;
	inputsD = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(excitation) * inputSize, NULL, &ret);
	handleError();
	outputsD = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(excitation) * inputSize, NULL, &ret);
	handleError();
	weightsD = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(weight) * outputSize * inputSize, NULL, &ret);
	handleError();
}

void OpenCLFullyConnectedLayer::deinit() {
	cl_int ret;
	ret = clReleaseMemObject(inputsD);
	handleError();
	ret = clReleaseMemObject(outputsD);
	handleError();
	ret = clReleaseMemObject(weightsD);
	handleError();
}

OpenCLFullyConnectedLayer::OpenCLFullyConnectedLayer(vector<vector<weight> > &weightsVector) : Layer(
		OPENCL_FULLY_CONNECTED,
		(int) weightsVector[0].size(),
		(int) weightsVector.size()) {
	init((unsigned int) weightsVector.size(), (unsigned int) weightsVector[0].size());
	weight newWeights[outputSize][inputSize];
	for (int neuron = 0; neuron < weightsVector.size(); neuron++) {
		assert(weightsVector[neuron].size() == inputSize);
		for (unsigned int weightI = 0; weightI < inputSize; weightI++) {
			newWeights[neuron][weightI] = weightsVector[neuron][weightI];
		}
	}
	setNeuronWeights(outputSize, inputSize, (float *) newWeights);
}

OpenCLFullyConnectedLayer::OpenCLFullyConnectedLayer(unsigned int outputSize, unsigned int inputSize,
													 weight *newWeights) : Layer(OPENCL_FULLY_CONNECTED,
																				 inputSize,
																				 outputSize) {
	init(outputSize, inputSize);
	setNeuronWeights(outputSize, inputSize, newWeights);
}

OpenCLFullyConnectedLayer::OpenCLFullyConnectedLayer(const OpenCLFullyConnectedLayer &layer) : Layer(layer) {
	init(outputSize, inputSize);
	operator=(layer);
}

OpenCLFullyConnectedLayer::~OpenCLFullyConnectedLayer() {
	deinit();
}

void OpenCLFullyConnectedLayer::operator=(const OpenCLFullyConnectedLayer &layer) {
	//TODO Use simple assignment of vectors below
	Layer::operator=(layer);
	setInputs(layer.inputs);
	setOutputs(layer.outputs);
	weight newWeights[outputSize][inputSize];
	cl_int ret;
	ret = clEnqueueReadBuffer(cmdQ, layer.weightsD, CL_TRUE, 0, sizeof(weight) * outputSize * inputSize, newWeights, 0,
							  NULL,
							  NULL);
	handleError();
	ret = clEnqueueWriteBuffer(cmdQ, weightsD, CL_TRUE, 0, sizeof(weight) * outputSize * inputSize, newWeights, 0, NULL,
							   NULL);
	handleError();
}

vector<vector<weight> > OpenCLFullyConnectedLayer::getNeuronWeights() {
	weight newWeights[outputSize][inputSize];
	cl_int ret;
	ret = clEnqueueReadBuffer(cmdQ, weightsD, CL_TRUE, 0, sizeof(weight) * outputSize * inputSize, newWeights, 0, NULL,
							  NULL);
	handleError();

	vector<vector<weight> > returnWeights(outputSize, vector<weight>(inputSize));
	for (unsigned int i = 0; i < outputSize; i++) {
		for (unsigned int j = 0; j < inputSize; j++) {
			returnWeights[i][j] = newWeights[i][j];
		}
	}
	return returnWeights;
}

void OpenCLFullyConnectedLayer::setNeuronWeights(vector<vector<weight> > &newWeightVector) {
	weight newWeights[outputSize][inputSize];
	for (unsigned int outputI = 0; outputI < outputSize; outputI++) {
		for (unsigned int inputI = 0; inputI < inputSize; inputI++) {
			newWeights[outputI][inputI] = newWeightVector[outputI][inputI];
		}
	}
	cl_int ret;
	ret = clEnqueueWriteBuffer(cmdQ, weightsD, CL_TRUE, 0, sizeof(weight) * outputSize * inputSize, newWeights, 0, NULL,
							   NULL);
	handleError();
}

void OpenCLFullyConnectedLayer::setNeuronWeights(unsigned int outputSize, unsigned int inputSize, weight *newWeights) {
	assert(this->outputSize == outputSize);
	assert(this->inputSize == inputSize);
	cl_int ret = clEnqueueWriteBuffer(cmdQ, weightsD, CL_TRUE, 0, sizeof(weight) * outputSize * inputSize, newWeights,
									  0, NULL,
									  NULL);
	handleError();
}

void OpenCLFullyConnectedLayer::setInputs(const vector<excitation> &inputs) {
	assert(inputSize == inputs.size());
	Layer::setInputs(inputs);
	excitation excitations[inputSize];
	for (unsigned int i = 0; i < inputSize; i++) {
		excitations[i] = inputs[i];
	}
	cl_int ret = clEnqueueWriteBuffer(cmdQ, inputsD, CL_TRUE, 0, inputSize * sizeof(excitation),
									  excitations, 0, NULL,
									  NULL);
	handleError();
}

void OpenCLFullyConnectedLayer::setOutputs(const vector<excitation> &outputs) {
	assert(outputSize == outputs.size());
	Layer::setOutputs(outputs);
	excitation excitations[outputSize];
	for (unsigned int i = 0; i < outputSize; i++) {
		excitations[i] = outputs[i];
	}
	cl_int ret = clEnqueueWriteBuffer(cmdQ, outputsD, CL_TRUE, 0, outputSize * sizeof(excitation), excitations, 0, NULL,
									  NULL);
	handleError();
}

Layer *OpenCLFullyConnectedLayer::getDeepCopy() {
	return new OpenCLFullyConnectedLayer(*this);
}

void OpenCLFullyConnectedLayer::compute() {
	cl_int ret;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputsD);
	handleError();
	ret = clSetKernelArg(kernel, 1, sizeof(unsigned int), &inputSize);
	handleError();
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &weightsD);
	handleError();
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &outputsD);
	handleError();
	size_t globalSize[] = {outputSize};
	ret = clEnqueueNDRangeKernel(cmdQ, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
	handleError();

	excitation excitations[outputSize];
	ret = clEnqueueReadBuffer(cmdQ, outputsD, CL_TRUE, 0, outputSize * sizeof(excitation), &excitations[0], 0, NULL,
							  NULL);
	handleError();
	for (unsigned int i = 0; i < outputSize; i++) {
		outputs[i] = excitations[i];
		//cout << outputs[i] << '\n';
	}
}


void OpenCLFullyConnectedLayer::save(ostream &file) {
	file <= type <= inputSize <= outputSize;
	cl_int ret;
	weight newWeights[outputSize][inputSize];
	ret = clEnqueueReadBuffer(cmdQ, weightsD, CL_TRUE, 0, sizeof(weight) * outputSize * inputSize, newWeights, 0, NULL,
							  NULL);
	handleError();
	for (unsigned long i = 0; i < outputSize; i++) {
		for (unsigned int j = 0; j < inputSize; j++) {
			file <= newWeights[i][j];
		}
	}
}

OpenCLFullyConnectedLayer OpenCLFullyConnectedLayer::getLoadedLayer(istream &file) {
	unsigned int inputSize, outputSize;
	file >= inputSize >= outputSize;
	weight newWeights[outputSize][inputSize];
	for (unsigned long i = 0; i < outputSize; i++) {
		for (unsigned int j = 0; j < inputSize; j++) {
			file >= newWeights[i][j];
		}
	}
	OpenCLFullyConnectedLayer layer(outputSize, inputSize, (weight *) newWeights);
	return layer;
}