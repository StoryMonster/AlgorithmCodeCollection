#pragma once

#include "Messages.h"
#include <set>

class Elevator
{
public:
	Elevator(const unsigned int aId, const unsigned int aFloorCount);
    Elevator(const unsigned int aId, const unsigned int aFloorCount, const unsigned int aCurrentFloor, const Direction aDirection);

	void SelectFloor(const unsigned int aFloorId);

	unsigned int CurrentFloor() const;
	Direction CurrentDirection() const;
	bool HasWork() const;

	bool CanRespondToCall(const MessageElevatorCall&) const;
	void RespondToCall(const MessageElevatorCall&);

	bool IsStoppableCallingFloor() const;
	bool IsRequestFloor() const;
	void HandleArrivedAtRequestFloor();
	void HandleArrivedAtCallingFloor();

	void Step();
	unsigned int Id() const;

	std::string ToString() const;

private:
	unsigned int myId;
	unsigned int myFloorCount;
	unsigned int myCurrentFloor;
	Direction myCurrentDirection;
	std::set<unsigned int> requestFloors{};
	std::set<MessageElevatorCall> calls{};
};
