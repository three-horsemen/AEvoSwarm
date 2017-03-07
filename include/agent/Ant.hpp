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

using namespace std;
namespace ant {
	namespace sensor {
		enum Sensor {
			FRONT,
			LEFT,
			RIGHT
		};
	}
	namespace percept {
		enum Percept {
			ATTITUDE,
			TRAIT,
			ENERGY
		};
	}
	namespace adjacency {
		enum Adjacency {
			AHEAD,
			LEFT,
			RIGHT,
			BEHIND
		};
	}
	namespace action {
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
	}
}
using namespace ant;

class Ant : public Agent {
public:
	static short actionCost[10];
protected:
	Energy potential;
	Energy shield;
	Energy fertility;
	Energy baby;
	AgentCharacter character;

	Coordinate getCoordinate(Coordinate, Occupancy, adjacency::Adjacency);

	Coordinate getCoordinate(Occupancy, adjacency::Adjacency);

	Coordinate getCoordinate(adjacency::Adjacency);


public:
	Ant(Coordinate, Energy, Energy, Energy, Energy, AgentCharacter);

	bool isEnergyAvailable(Agent::Action action);

	bool isActionValid(Agent::Action action);

	void observeEnvironment(Environment &);

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

	void moveForward(Environment &);

	void turnLeft(Environment &);

	void turnRight(Environment &);


	static Ant generateRandomAnt();

	void placeAntInEnvironment(Environment &, Coordinate);

	int calculateDistance(Coordinate, Coordinate);

	excitation getSensation(sensor::Sensor, percept::Percept);
};


#endif //SFTP_ANT_HPP
