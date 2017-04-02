//
// Created by reuben on 2/4/17.
//

#ifndef AEVO_BRAINSCORES_HPP
#define AEVO_BRAINSCORES_HPP


#include <brain/BrainLoss.hpp>
#include <brain/NeuralNetwork.hpp>

class BrainLosses {
public:
	vector<BrainLoss> topScores;
	vector<NeuralNetwork> &brains;

	BrainLosses(vector<NeuralNetwork> &brains) : topScores(brains.size()), brains(brains) {
		for (unsigned int i = 0; i < brains.size(); ++i) {
			topScores[i].index = i;
			topScores[i].ancestorID = 10000000 / brains.size() * i;
		}
	}

	void addLosses(vector<NeuralNetwork> &brains, vector<excitation> trueOutputs) {
		for (unsigned int x = 0; x < topScores.size(); ++x) {
			int i = topScores[x].index;
			vector<excitation> outputs = brains[i].getOutputLayer()->getOutputs();
			float score = BrainLoss::computeScore(outputs, trueOutputs);
			topScores[x].totalLoss += score;
			topScores[x].lossCount++;
		}
	}

	void consolidateLosses() {
		for (unsigned int x = 0; x < topScores.size(); ++x) {
			topScores[x].totalLoss /= topScores[x].lossCount;
			topScores[x].lossCount = 1;
		}
		sort(topScores.begin(), topScores.end(), BrainLoss::byScoreAsc());
	}

	void populateNextGeneration(vector<NeuralNetwork> &brains, float rate) {
		unsigned int parentCount = (unsigned int) (topScores.size() * rate);
		vector<vector<vector<vector<weight> > > > brainWeights
				(parentCount,
				 vector<vector<vector<weight> > >
						 (brains[0].getDepth(),
						  vector<vector<weight> >(brains[0].getLayer(1)->getOutputs().size(),
												  vector<weight>(brains[0].getLayer(1)->getInputs().size()
												  )
						  )
						 )
				);
		for (unsigned int x = 0; x < parentCount; ++x) {
			assert(topScores[x].index != -1);
			unsigned int i = topScores[x].index;
			NeuralNetwork &brain = brains[i];
			for (int j = 0; j < brain.getDepth(); j++) {
				if (brain.getLayer(j)->type == FULLY_CONNECTED) {
					FullyConnectedLayer *layer = (FullyConnectedLayer *) brain.getLayer(j);
					for (int k = 0; k < layer->outputSize; k++) {
						brainWeights[x][j][k] = layer->getNeuronWeights(k);
					}
				}
			}
		}
		for (unsigned int child = 0; child < brains.size(); ++child) {
			unsigned long parent = child % parentCount;
			if (child != parent)
				topScores[child].ancestorID = topScores[parent].ancestorID + 1;
			NeuralNetwork &brain = brains[child];
			for (int j = 0; j < brain.getDepth(); j++) {
				if (brain.getLayer(j)->type == FULLY_CONNECTED) {
					FullyConnectedLayer *layer = (FullyConnectedLayer *) brain.getLayer(j);
					for (int k = 0; k < layer->outputSize; k++) {
						layer->setNeuronWeights(k, brainWeights[parent][j][k]);
					}
				}
			}
		}
	}

	friend ostream &operator<<(ostream &os, const BrainLosses &brainScores) {
		for (unsigned int i = 0; i < brainScores.topScores.size(); i++) {
			assert(brainScores.topScores[i].index != -1);
			cout << brainScores.topScores[i] << ", ";
		}
		return os;
	}

	float getLossMean() {
		float accum = 0.0;
		std::for_each(std::begin(topScores), std::end(topScores), [&](const BrainLoss d) {
			accum += d.totalLoss;
		});
		return accum / topScores.size();
	}

	float getLossVariance(float mean) {
		float accum = 0.0;
		std::for_each(std::begin(topScores), std::end(topScores), [&](const BrainLoss d) {
			accum += (d.totalLoss - mean) * (d.totalLoss - mean);
		});
		return accum / topScores.size();
	}

	float getMutationRateMean() {
		float accum = 0.0;
		std::for_each(std::begin(topScores), std::end(topScores), [&](const BrainLoss d) {
			accum += d.mutationRate;
		});
		return accum / topScores.size();
	}

	float getMutationRateVariance(float mean) {
		float accum = 0.0;
		std::for_each(std::begin(topScores), std::end(topScores), [&](const BrainLoss d) {
			accum += (d.mutationRate - mean) * (d.mutationRate - mean);
		});
		return accum / topScores.size();
	}

};

#endif //AEVO_BRAINSCORES_HPP
