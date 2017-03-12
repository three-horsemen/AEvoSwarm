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
	namespace adjacency {
		enum Adjacency {
			UNDER,
			AHEAD,
			LEFT,
			RIGHT,
			BEHIND
		};
		const short ADJACENCY_COUNT = 4;
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
	static const Energy NEWBORN_MIN_SHIELD = MAX_DAMAGE * 4;
	static const Energy NEWBORN_MIN_POTENTIAL;
	static const Energy NEWBORN_MIN_TOTAL_ENERGY;
protected:
	Energy potential;
	Energy shield;
	Energy fertility;
	Energy fetal;

	AgentCharacter character;
	//TODO Use Boost enum
	static const short actionCount, senseCount = 15, memoryCount = 15;

	vector<excitation> sensoryInputs;
	static const int GROUND_ATTITUDE = 0, GROUND_TRAIT = 0;
	static constexpr float FORTIFY_FACTOR = 0.05f, MATURE_FACTOR = 0.05f, FETAL_DEVELOPMENT_FACTOR = 0.05f;

	Energy pushedFetalEnergy;

	static Coordinate getCoordinate(Coordinate, Occupancy, adjacency::Adjacency);

	Coordinate getLocalCoordinate(Occupancy, adjacency::Adjacency);

	Coordinate getLocalCoordinate(adjacency::Adjacency);

	Coordinate getLocalCoordinate();

	bool isThereBirthSpace();

	void moveForward();

	void turnLeft();

	void turnRight();

	void eat();

	void attack();

	void beAttacked(Energy);

	void fortify();

	void mature();

	void growBaby();

	void pushOutNewborn();

	void die();

	Tile operator<<(Tile);

	Tile operator>>(Tile);

	Tile operator>=(Tile);

	Tile operator<=(Tile);

	Tile operator>(Tile);

	Tile operator<(Tile);

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

	static void affectEnvironment(vector<Ant> &, unsigned short, Environment &);

	static void eraseDeadAnts(vector<Ant> &);

	static void realizeAntsAction(vector<Ant> &, Environment &);

	void pullOutNewborn(Environment &, Ant &);

	void mutate();

	static Coordinate getGlobalCoordinate(Environment &, Coordinate, Occupancy, adjacency::Adjacency);

	Coordinate getGlobalCoordinate(Environment &, Occupancy, adjacency::Adjacency);

	Coordinate getGlobalCoordinate(Environment &, adjacency::Adjacency);

	Coordinate getGlobalCoordinate();

	Energy getPotential();

	Energy getShield();

	Energy getFertility();

	Energy getFetal();

	AgentCharacter getCharacter();

	void setGlobalCoordinate(Coordinate);

	void setPotential(Energy newPotential);

	void setShield(Energy newShield);

	void setFertility(Energy newFertility);

	void setFetal(Energy newBaby);

	void setPushedFetalEnergy(Energy);

	void setCharacter(AgentCharacter newCharacter);

	Energy getTotalEnergy();

	void dissipateEnergy(Energy);

	void randomize();

	static void placeInEnvironment(Ant &, Environment &, Coordinate);

	static void placeCharacterInEnvironment(Ant &, Environment &, Coordinate);

	int calculateDistance(Coordinate, Coordinate);

	excitation getMaxPerceptValue(percept::Percept);

	excitation getSensation(sensor::Sensor, percept::Percept);
};


#endif //AEVO_ANT_HPP
