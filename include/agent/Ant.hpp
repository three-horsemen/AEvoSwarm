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

class Ant : public Agent {
protected:
	Energy potential;
	Energy shield;
	Energy fertility;
	Energy baby;
	AgentCharacter character;

	static Coordinate getCoordinateAhead(Coordinate, Occupancy);

	Coordinate getCoordinateAhead(Occupancy);

	Coordinate getCoordinateAhead();

	static Coordinate getCoordinateBehind(Coordinate, Occupancy);

	Coordinate getCoordinateBehind(Occupancy);

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
	static short actionCost[10];

	Ant();

	Ant(Coordinate, Energy, Energy, Energy, Energy, AgentCharacter);

	bool isEnergyAvailable(Agent::Action action);

	bool isActionValid(Agent::Action action);

	void developBrain();

	void observeEnvironment(Environment &);

	Agent::Action getSelectAction();

	Agent::Action performAction(Agent::Action);

	void affectEnvironment(Environment &);

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
};


#endif //SFTP_ANT_HPP
