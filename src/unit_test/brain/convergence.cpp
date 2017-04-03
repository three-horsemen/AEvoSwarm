//
// Created by reuben on 5/3/17.
//

#include <brain/NeuralNetwork.hpp>
#include <brain/BrainLosses.hpp>

void mutate(BrainLoss &brainScore, vector<NeuralNetwork> &brains) {
	NeuralNetwork &brain = brains[brainScore.index];
	float r = ((rand() % 201 - 100) / 500.f);
	brainScore.mutationRate *= 1 + r * r * r;
	for (int i = 0; i < brain.getDepth(); i++) {
		if (brain.getLayer(i)->type == FULLY_CONNECTED) {
			FullyConnectedLayer *layer = (FullyConnectedLayer *) brain.getLayer(i);
			for (int j = 0; j < layer->outputSize; j++) {
				vector<weight> weights = layer->getNeuronWeights(j);
				Neuron::mutateWeights(weights, brainScore.mutationRate);
				layer->setNeuronWeights(j, weights);
			}
		}
	}
}

int main(int argc, char *argv[]) {

	const short inputSize = 25, fC1Size = 24, fC2Size = 16, outputSize = 10;
	InputLayer inputLayer(inputSize);
	vector<excitation> excitations(inputSize);
	Neuron::randomizeExcitation(excitations);
	inputLayer.setInputs(excitations);

	vector<vector<weight> > weights1(fC1Size, vector<weight>(inputSize));
	Neuron::randomizeWeights(weights1);
	FullyConnectedLayer fullyConnectedLayer1(weights1);

	vector<vector<weight> > weights2(fC2Size, vector<weight>(fC1Size));
	Neuron::randomizeWeights(weights2);
	FullyConnectedLayer fullyConnectedLayer2(weights2);

	vector<vector<weight> > weights3(outputSize, vector<weight>(fC2Size));
	Neuron::randomizeWeights(weights3);
	FullyConnectedLayer fullyConnectedLayer3(weights3);
	OutputLayer outputLayer(outputSize);

	const unsigned int brainCount = 20;
	vector<NeuralNetwork> brains(brainCount);
	for (unsigned int i = 0; i < brains.size(); i++) {
		brains[i].addLayer((Layer &) inputLayer);
		brains[i].addLayer((Layer &) fullyConnectedLayer1);
		brains[i].addLayer((Layer &) fullyConnectedLayer2);
		brains[i].addLayer((Layer &) fullyConnectedLayer3);
		brains[i].addLayer((Layer &) outputLayer);
	}

	{
		ifstream convergedBrainFile("convergedBrain.brain");
		brains[0].load(convergedBrainFile);
		convergedBrainFile.close();
	}

	BrainLosses topBrains(brains);
	NeuralNetwork trueBrain;
	trueBrain.addLayer((Layer &) inputLayer);
	Neuron::randomizeWeights(weights1);
	FullyConnectedLayer fullyConnectedLayer1_(weights1);
	trueBrain.addLayer((Layer &) fullyConnectedLayer1_);
	Neuron::randomizeWeights(weights2);
	FullyConnectedLayer fullyConnectedLayer2_(weights2);
	trueBrain.addLayer((Layer &) fullyConnectedLayer2_);
	Neuron::randomizeWeights(weights3);
	FullyConnectedLayer fullyConnectedLayer3_(weights3);
	trueBrain.addLayer((Layer &) fullyConnectedLayer3_);
	trueBrain.addLayer((Layer &) outputLayer);

	for (int i = 0; i < 1000000; i++) {
		Neuron::randomizeExcitation(excitations);
		trueBrain.getLayer(0)->setInputs(excitations);
		trueBrain.compute();
		for (unsigned int j = 0; j < brains.size(); j++) {
			//brains[j].mutationRate = 0.006f;
			brains[j].getLayer(0)->setInputs(excitations);
			brains[j].compute();
//			brains[j].getOutputLayer()->displayOutputs();
//			cout << endl;
		}
		topBrains.addLosses(brains, trueBrain.getOutputLayer()->getOutputs());
		if (i % 1600 == 0) {
			topBrains.consolidateLosses();
			topBrains.populateNextGeneration(brains, 0.1);
			for (unsigned int j = 0; j < topBrains.topScores.size(); j++) {
				mutate(topBrains.topScores[j], brains);
			}
		}
		if (i % 1600 == 0) {
			float avg = topBrains.getLossMean(), avgMutation = topBrains.getMutationRateMean();
			cout << '(' << avg << ", " << topBrains.getLossVariance(avg)
				 << ") , (" << avgMutation << ", " << topBrains.getMutationRateVariance(avgMutation) << ")"
				 << endl;
		}
	}

	{
		ofstream convergedBrainFile("convergedBrain.brain");
		brains[topBrains.topScores[0].index].save(convergedBrainFile);
		convergedBrainFile.close();
	}

	//Layer* layer = &outputLayer;
	//layer->displayOutputs();

	return 0;
}
