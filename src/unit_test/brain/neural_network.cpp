//
// Created by reuben on 5/3/17.
//

#include <brain/NeuralNetwork.hpp>

int main(int argc, char *argv[]) {

	NeuralNetwork brain;

	const short inputSize = 25, fC1Size = 24, fC2Size = 16, outputSize = 10;
	InputLayer inputLayer(inputSize);
	vector<excitation> excitations(inputSize);
	Neuron::randomizeExcitation(excitations);
	inputLayer.setInputs(excitations);
	brain.addLayer((Layer &) inputLayer);

	vector<vector<weight> > weights1(fC1Size, vector<weight>(inputSize));
	Neuron::randomizeWeights(weights1);
	FullyConnectedLayer fullyConnectedLayer1(weights1);
	brain.addLayer((Layer &) fullyConnectedLayer1);

	vector<vector<weight> > weights2(fC2Size, vector<weight>(fC1Size));
	Neuron::randomizeWeights(weights2);
	FullyConnectedLayer fullyConnectedLayer2(weights2);
	brain.addLayer((Layer &) fullyConnectedLayer2);

	vector<vector<weight> > weights3(outputSize, vector<weight>(fC2Size));
	Neuron::randomizeWeights(weights3);
	FullyConnectedLayer fullyConnectedLayer3(weights3);
	brain.addLayer((Layer &) fullyConnectedLayer3);

	OutputLayer outputLayer(outputSize);
	brain.addLayer((Layer &) outputLayer);

	for (int i = 0; i < 1000000; i++) {
		Neuron::randomizeExcitation(excitations);
		inputLayer.setInputs(excitations);
		brain.compute();
		//brain.getOutputLayer()->displayOutputs();
		//cout<<endl;
	}
	brain.getOutputLayer()->displayOutputs();

	//Layer* layer = &outputLayer;
	//layer->displayOutputs();

	return 0;
}