#pragma once

#include "Messages.h"
#include "Utils.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <set>

enum Direction;

class Elevator
{
public:
	Elevator(const unsigned int aId, const unsigned int aFloorCount);
    Elevator(const unsigned int aId, const unsigned int aFloorCount, const unsigned int aCurrentFloor, const Direction aDirection);

	void SelectFloor(const unsigned int aFloorId);

	unsigned int CurrentFloor() const;
	Direction CurrentDirection() const;
	bool HasWork() const;
	bool WillStopAtCurrentFloor() const;
	void StopAtCurrentFloor() const;
	bool CanResponseElevatorCall(const MessageElevatorCall&) const;

	void Step();
	unsigned int Id() const;

	std::string ToString() const;

private:
	unsigned int myId;
	unsigned int myFloorCount;
	unsigned int myCurrentFloor;
	Direction myCurrentDirection;
	std::set<unsigned int> floorsNeedStop{};
};
