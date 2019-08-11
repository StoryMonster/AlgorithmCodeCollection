#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>
#include "Messages.h"

enum HumanState
{
	HumanState_Idle = 0,
	HumanState_Waiting,
	HumanState_Traveling,
	HumanState_Arrived
};

class Human
{
public:
    Human(const unsigned int aFloor, const unsigned int	aDestinationFloor);

	unsigned int myFloor;
	unsigned int myDestinationFloor;

	HumanState GetState() const;
	void SetStateWaiting();
	void SetStateTraveling();
	void SetStateArrived();

	void Step();
	unsigned int GetWaitingCounter() const;
	unsigned int GetTravelingCounter() const;

	void OnElevatorArrived(const MessageElevatorArrived&);

private:
	HumanState myState;
	unsigned int myWaitingCounter;
	unsigned int myTravelingCounter;
	unsigned int myElevatorId;
};

class Humans
{
public:
    Humans();

	void Start();

	void OnMessageElevatorReady(const MessageElevatorReady& aMessage);
	void OnMessageElevatorArrived(const MessageElevatorArrived&	aMessage);
	void OnMessageHumanStep(const MessageHumanStep& aMessage);

private:
	std::vector<Human> myHumans;
	void PrivPrintTimers();
};
