//
// Created by reuben on 5/3/17.
//

#include <ant/NeuralNetwork.hpp>
#include <ant/InputLayer.hpp>
#include <ant/OutputLayer.hpp>

int main(int argc, char *argv[]) {
	srand((unsigned int) time(NULL));

	NeuralNetwork brain;

	const short inputSize = 25, fC1Size = 24, fC2Size = 16, outputSize = 10;
	InputLayer inputLayer(inputSize);
	excitation excitations[inputSize];
	neuron::randomizeExcitation(inputSize, excitations);
	inputLayer.setInputs((excitation *) excitations);
	brain.addLayer((Layer &) inputLayer);

	weight weights1[fC1Size][inputSize];
	neuron::randomizeWeights(inputSize, fC1Size, (weight *) weights1);
	FullyConnectedLayer fullyConnectedLayer1(inputSize, fC1Size, (weight *) weights1);
	brain.addLayer((Layer &) fullyConnectedLayer1);

	weight weights2[fC2Size][fC1Size];
	neuron::randomizeWeights(fC1Size, fC2Size, (weight *) weights2);
	FullyConnectedLayer fullyConnectedLayer2(fC1Size, fC2Size, (weight *) weights2);
	brain.addLayer((Layer &) fullyConnectedLayer2);

	weight weights3[outputSize][fC2Size];
	neuron::randomizeWeights(fC2Size, outputSize, (weight *) weights3);
	FullyConnectedLayer fullyConnectedLayer3(fC2Size, outputSize, (weight *) weights3);
	brain.addLayer((Layer &) fullyConnectedLayer3);

	OutputLayer outputLayer(outputSize);
	brain.addLayer((Layer &) outputLayer);

	for (int i = 0; i < 1000000; i++) {
		neuron::randomizeExcitation(inputSize, excitations);
		inputLayer.setInputs((excitation *) excitations);
		brain.compute();
		//brain.getOutputLayer()->displayOutputs();
		//cout<<endl;
	}
	brain.getOutputLayer()->displayOutputs();

	//Layer* layer = &outputLayer;
	//layer->displayOutputs();

	return 0;
}