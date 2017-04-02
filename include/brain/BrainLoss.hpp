//
// Created by reuben on 2/4/17.
//

#ifndef AEVO_BRAINSCORE_HPP
#define AEVO_BRAINSCORE_HPP

#include <brain/Neuron.hpp>
#include <limits>

using namespace std;

class BrainLoss {
public:
	unsigned int index = (unsigned int) -1;
	float totalLoss = 0;
	unsigned lossCount = 0;
	float mutationRate = 0.08f;
	unsigned long ancestorID = 0;

	static float computeScore(const vector<excitation> &trueOutputs, const vector<excitation> &outputs) {
		assert(outputs.size() == trueOutputs.size());
		float diff = 0;
		for (unsigned int i = 0; i < outputs.size(); ++i) {
			diff += (trueOutputs[i] - outputs[i]) * (trueOutputs[i] - outputs[i]);
		}
		return diff;
	}

	struct byScoreAsc {
		bool operator()(BrainLoss const &a, BrainLoss const &b) const {
			return a.totalLoss < b.totalLoss;
		}
	};

	friend ostream &operator<<(ostream &os, const BrainLoss &brainScore) {
		os << '{' << brainScore.index << ", " << brainScore.totalLoss << ", " << brainScore.mutationRate << '}';
		return os;
	}
};

#endif //AEVO_BRAINSCORE_HPP
