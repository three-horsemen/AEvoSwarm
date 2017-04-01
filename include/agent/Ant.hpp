//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_ANT_HPP
#define AEVO_ANT_HPP

#include <shared/logger.hpp>
#include <shared/Utils.hpp>

#include <agent/Agent.hpp>
#include <agent/AgentCharacter.hpp>

#include <environment/Coordinate.hpp>
#include <environment/Energy.hpp>

#include <brain/InputLayer.hpp>
#include <brain/OutputLayer.hpp>

#include <iostream>

using namespace std;

namespace ant {
	namespace sensor {
		enum Sensor {
			FRONT,
			LEFT,
			RIGHT
		};
		const short SENSOR_COUNT = 3;
	}
	namespace percept {
		enum Percept {
			ATTITUDE,
			TRAIT,
			ENERGY
		};
		const short PERCEPT_COUNT = 3;
	}
}
using namespace ant;

class Ant : public Agent {
public:
	static const Energy HYPOTHETICAL_MAX_POTENTIAL_ENERGY = 10000;
	static const Energy HYPOTHETICAL_MAX_SHIELD_ENERGY = 10000;
	static const Energy HYPOTHETICAL_MAX_FERTILITY_ENERGY = 10000;
	static const Energy HYPOTHETICAL_MAX_BABY_ENERGY = 10000;
	static const Energy HYPOTHETICAL_MAX_ENERGY =
			HYPOTHETICAL_MAX_POTENTIAL_ENERGY + HYPOTHETICAL_MAX_SHIELD_ENERGY + HYPOTHETICAL_MAX_FERTILITY_ENERGY +
			HYPOTHETICAL_MAX_BABY_ENERGY;
	static const Attitude HYPOTHETICAL_MAX_ATTITUDE = 256;
	static const Attitude HYPOTHETICAL_MIN_ATTITUDE = 128;
	static const Attitude HYPOTHETICAL_MAX_TRAIT = 256;
	static const int HYPOTHETICAL_MAX_OCCUPANCY_VAL = 4;

	static const Energy MAX_DAMAGE = 10;

	static const Energy NEWBORN_MIN_POTENTIAL;
	static const Energy NEWBORN_MIN_SHIELD = MAX_DAMAGE * 4;
	static const Energy NEWBORN_MIN_FERTILITY = 1;
	static const Energy NEWBORN_MIN_TOTAL_ENERGY;
protected:
	Energy potential;
	Energy shield;
	Energy fertility;
	Energy fetal;
	unsigned int age;
	unsigned short children, nGeneration;

	AgentCharacter character;
	//TODO Use Boost enum
	static const short actionCount, senseCount = 15, memoryCount = 5;

	vector<excitation> sensoryInputs;
	static const int GROUND_ATTITUDE = 0, GROUND_TRAIT = 0;
	static constexpr float FORTIFY_FACTOR = 0.05f, MATURE_FACTOR = 0.05f, FETAL_DEVELOPMENT_FACTOR = 0.05f;

	Energy pushedFetalEnergy;

	Coordinate getLocalCoordinate(Occupancy, adjacency::Adjacency);

	Coordinate getLocalCoordinate(adjacency::Adjacency);

	Coordinate getLocalCoordinate();

	bool isThereBirthSpace();

	void moveForward();

	void turnLeft();

	void turnRight();

	void eat();

	void attack();

	void fortify();

	void mature();

	void growBaby();

	void pushOutNewborn();

public:
	static vector<Energy> actionCost;

	enum Action {
		FORWARD, //0
		LEFT, //1
		RIGHT, //2
		EAT, //3
		ATTACK, //4
		FORTIFY, //5
		MATURE, //6
		GROW_BABY, //7
		GIVE_BIRTH, //8
		DIE //9
	};
	//TODO Add action to change ATTITUDE attribute of Character.

	Ant();

	Ant(const Ant &);

	~Ant();

	void operator=(const Ant &);

	static bool isInImpactRange(Environment &, Coordinate);

	bool isEnergyAvailable(Agent::Action action);

	bool isActionValid(Agent::Action action);

	void developBrain();

	void resorbBrain();

	void observeEnvironment(Environment &);

	void senseObservation(Environment &);

	void selectAction();

	Agent::Action getSelectedAction() const;

	void setSelectedAction(Agent::Action, bool = false);

	void performAction(Agent::Action);

	void pullOutNewborn(Environment &, Ant &);

	void beAttacked(Energy);

	void die();

	void mutate();

	Coordinate getGlobalCoordinate(Environment &, Occupancy, adjacency::Adjacency);

	Coordinate getGlobalCoordinate(Environment &, adjacency::Adjacency);

	Coordinate getGlobalCoordinate();

	inline Energy getActionCost(Agent::Action action) const {
		float energyScaleFactor;
		switch (action) {
			case FORWARD:
				energyScaleFactor = 10.f;
				break;
			default:
				energyScaleFactor = 2.f;
				break;
		}
//	return actionCost[action];
		return (Energy) (actionCost[action] *
						 (
								 (
										 1.f + energyScaleFactor *
											   ((float) getTotalEnergy() / getMaxPerceptValue(percept::ENERGY))
								 ) *
								 (
										 1.f + 1000000.f * ((float) age / ((unsigned int) -1))
								 )
						 ));
	}

	inline Energy getPotential() const {
		return potential;
	}

	inline Energy getShield() const {
		return shield;
	}

	inline Energy getFertility() const {
		return fertility;
	}

	inline Energy getFetal() const {
		return fetal;
	}

	inline unsigned int getAge() const {
		return age;
	}

	inline unsigned int getChildrenCount() const {
		return children;
	}

	inline unsigned int getGenerationCount() const {
		return nGeneration;
	}

	inline AgentCharacter getCharacter() {
		return character;
	}

	void setGlobalCoordinate(Coordinate newCoordinate) {
		globalCoordinate = newCoordinate;
	}

	inline void setPotential(Energy newPotential) {
		potential = newPotential;
	}

	void setShield(Energy newShield) {
		shield = newShield;
	}

	void setFertility(Energy newFertility) {
		fertility = newFertility;
	}

	void setFetal(Energy newFetal) {
		fetal = newFetal;
	}

	void setPushedFetalEnergy(Energy newPushedFetalEnergy) {
		pushedFetalEnergy = newPushedFetalEnergy;
	}

	inline void setCharacter(AgentCharacter newCharacter) {
		character.setAttitude(newCharacter.getAttitude());
		character.setOccupancy(newCharacter.getOccupancy());
		character.setTrait(newCharacter.getTrait());
	}

	inline Energy getTotalEnergy() const {
		return potential + shield + fertility + fetal;
	}

	void dissipateEnergy(Energy);

	Tile operator<<(Tile);

	Tile operator>>(Tile);

	Tile operator>=(Tile);

	Tile operator<=(Tile);

	Tile operator>(Tile);

	Tile operator<(Tile);

	void randomize();

	int calculateDistance(Coordinate, Coordinate);

	static inline excitation getMaxPerceptValue(percept::Percept percept) {
//	unsigned long maxPerceptVal = 0;
		if (percept == percept::ENERGY) {
			return (Energy) -1;
//		Energy e = 0;
//		do {
//			e--;
//		} while (e < 0);
//		maxPerceptVal = e;
		} else if (percept == percept::ATTITUDE) {
			return (Attitude) -1;
//		Attitude e = 0;
//		do {
//			e--;
//		} while (e < 0);
//		maxPerceptVal = e;
		} else if (percept == percept::TRAIT) {
			return (Trait) -1;
//		Trait e = 0;
//		do {
//			e--;
//		} while (e < 0);
//		maxPerceptVal = e;
		} else {
			throw invalid_argument("Unknown percept");
		}
//	return (excitation) maxPerceptVal;
	}

	excitation getSensation(sensor::Sensor, percept::Percept);

	excitation getEnergyExcitation(Energy energy, Energy maxEnergy) {
		return (excitation) (log(energy + 1) / log(maxEnergy + 1)) - 1;
	}

	void save(ostream &);

	void saveWithCharacter(ostream &);

	void load(istream &, Environment &);

	void loadWithCharacter(istream &);
};


#endif //AEVO_ANT_HPP
