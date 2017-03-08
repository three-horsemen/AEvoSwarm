//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_ANT_HPP
#define SFTP_ANT_HPP

#include <shared/logger.hpp>
#include <shared/Utils.hpp>

#include <agent/Agent.hpp>
#include <agent/AgentCharacter.hpp>

#include <environment/Coordinate.hpp>
#include <environment/Energy.hpp>

#include <brain/InputLayer.hpp>
#include <brain/OutputLayer.hpp>

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
	Energy baby;
	AgentCharacter character;

	static short actionCost[10];
	excitation *sensoryInputs;

	Coordinate getCoordinate(Coordinate, Occupancy, adjacency::Adjacency);

	Coordinate getCoordinate(Occupancy, adjacency::Adjacency);

	Coordinate getCoordinate(adjacency::Adjacency);


	void moveForward();

	void turnLeft();

	void turnRight();

public:

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

	Ant();

	Ant(Coordinate, Energy, Energy, Energy, Energy, AgentCharacter);

	~Ant();

	bool isEnergyAvailable(Agent::Action action);

	bool isActionValid(Agent::Action action);

	void developBrain();

	void resorbBrain();

	void observeEnvironment(Environment &);

	void senseObservation(Environment &);

	Agent::Action getSelectedAction();

	void performAction(Agent::Action);

	void affectEnvironment(Environment &);

	static void realizeAntsAction(vector<Ant> &, Environment &);

	Coordinate getCoordinate();

	Energy getPotential();

	Energy getShield();

	Energy getFertility();

	Energy getBaby();

	AgentCharacter getCharacter();

	void setCoordinate(Coordinate);

	void setPotential(Energy newPotential);

	void setShield(Energy newShield);

	void setFertility(Energy newFertility);

	void setBaby(Energy newBaby);

	void setCharacter(AgentCharacter newCharacter);

	Energy getTotalEnergy();

	Tile operator<<(Tile);

	Tile operator>>(Tile);

	Tile operator>(Tile);

	Tile operator<(Tile);

	void randomize();

	void placeAntInEnvironment(Environment &, Coordinate);

	int calculateDistance(Coordinate, Coordinate);

	unsigned long getMaxPerceptValue(percept::Percept);

	excitation getSensation(sensor::Sensor, percept::Percept);
};


#endif //SFTP_ANT_HPP
