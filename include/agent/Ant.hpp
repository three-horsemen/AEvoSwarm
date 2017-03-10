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
protected:
	Energy potential;
	Energy shield;
	Energy fertility;
	Energy fetal;
	AgentCharacter character;

	//TODO Use Boost enum
	static const short actionCount, senseCount = 15, memoryCount = 15;
	vector<excitation> sensoryInputs;

	static const Energy MAX_DAMAGE = 10;
	static const Energy NEWBORN_SHIELD = MAX_DAMAGE * 4;
	static const Energy NEWBORN_POTENTIAL;
	static const Energy NEWBORN_TOTAL_ENERGY;
	static const int GROUND_ATTITUDE = 0, GROUND_TRAIT = 0;
	static constexpr float FORTIFY_FACTOR = 0.05f, MATURE_FACTOR = 0.05f, FETAL_DEVELOPMENT_FACTOR = 0.05f;

	Energy bornFetalEnergy;

	static Coordinate getCoordinate(Coordinate, Occupancy, adjacency::Adjacency);

	Coordinate getLocalCoordinate(Occupancy, adjacency::Adjacency);

	Coordinate getLocalCoordinate(adjacency::Adjacency);

	Coordinate getLocalCoordinate();


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
		FORWARD,
		LEFT,
		RIGHT,
		EAT,
		ATTACK,
		FORTIFY,
		MATURE,
		GROW_BABY,
		GIVE_BIRTH,
		DIE
	};
	//TODO Add action to change ATTITUDE attribute of Character.

	Ant();

	Ant(const Ant &);

	~Ant();

	void operator=(const Ant &);

	bool isEnergyAvailable(Agent::Action action);

	bool isActionValid(Agent::Action action);

	void developBrain();

	void resorbBrain();

	void observeEnvironment(Environment &);

	void senseObservation(Environment &);

	void selectAction();

	Agent::Action getSelectedAction();

	void performAction(Agent::Action);

	void affectEnvironment(vector<Ant> &, Environment &);

	static void eraseDeadAnts(vector<Ant> &);

	static void realizeAntsAction(vector<Ant> &, Environment &);

	Ant pullOutNewborn();

	void mutate();

	Coordinate getGlobalCoordinate(Occupancy, adjacency::Adjacency);

	Coordinate getGlobalCoordinate(adjacency::Adjacency);

	Coordinate getGlobalCoordinate();

	Energy getPotential();

	Energy getShield();

	Energy getFertility();

	Energy getFetal();

	AgentCharacter getCharacter();

	void setCoordinate(Coordinate);

	void setPotential(Energy newPotential);

	void setShield(Energy newShield);

	void setFertility(Energy newFertility);

	void setFetal(Energy newBaby);

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
