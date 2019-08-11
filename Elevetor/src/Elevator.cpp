#include "Elevator.h"
#include <cassert>
#include "Utils.h"

Elevator::Elevator(const unsigned int aId, const unsigned int aFloorCount)
	: myId(aId)
	, myFloorCount(aFloorCount)
	, myCurrentFloor(1)
	, myCurrentDirection(Direction::Up)
{
	Log("[Elevator]", myId, "Created", ToString());
}

Elevator::Elevator(
	const unsigned int aId,
	const unsigned int aFloorCount,
	const unsigned int aCurrentFloor,
	const Direction	aDirection)
	: myId(aId)
	, myFloorCount(aFloorCount)
	, myCurrentFloor(aCurrentFloor)
	, myCurrentDirection(aDirection)
{
	Log("[Elevator]", myId, "Created", ToString());
	if (aCurrentFloor == aFloorCount)
	{
		assert(aDirection == Direction::Down);
	}
	if (aCurrentFloor == 1)
	{
		assert(aDirection == Direction::Up);
	}
}

void Elevator::SelectFloor(const unsigned int aFloorId)
{
	// Implement me!
	if (!HasWork())
	{
		myCurrentDirection = (aFloorId > myCurrentFloor) ? Direction::Up : Direction::Down;
	}
    floorsNeedStop.emplace(aFloorId);
}

unsigned int Elevator::CurrentFloor() const
{
	return myCurrentFloor;
}

Direction Elevator::CurrentDirection() const
{
	return myCurrentDirection;
}

bool Elevator::HasWork() const
{
	// Implement me!
	return !floorsNeedStop.empty();
}

void Elevator::Step()
{
	// Implement me!
	if (!HasWork()) { return; }
	if (myCurrentDirection == Direction::Down)
	{
		if (std::find_if(floorsNeedStop.cbegin(), floorsNeedStop.cend(), [this](const auto floorId){ return floorId < this->myCurrentFloor;}) == floorsNeedStop.cend())
		{
			myCurrentDirection = Direction::Up;
			return;
		}
		--myCurrentFloor;
	}
	else
	{
		if (std::find_if(floorsNeedStop.cbegin(), floorsNeedStop.cend(), [this](const auto floorId){ return floorId > this->myCurrentFloor;}) == floorsNeedStop.cend())
		{
			myCurrentDirection = Direction::Down;
			return;
		}
		++myCurrentFloor;
	}
}

bool Elevator::WillStopAtCurrentFloor() const
{
    return floorsNeedStop.find(myCurrentFloor) != floorsNeedStop.cend();
}

void Elevator::StopAtCurrentFloor()
{
	if (myCurrentDirection == Direction::Down)
	{
		if (std::find_if(floorsNeedStop.cbegin(), floorsNeedStop.cend(), [this](const auto floorId){ return floorId < this->myCurrentFloor;}) == floorsNeedStop.cend())
		{
			myCurrentDirection = Direction::Up;
		}
	}
	else
	{
		if (std::find_if(floorsNeedStop.cbegin(), floorsNeedStop.cend(), [this](const auto floorId){ return floorId > this->myCurrentFloor;}) == floorsNeedStop.cend())
		{
			myCurrentDirection = Direction::Down;
		}
	}
    floorsNeedStop.erase(myCurrentFloor);
}

unsigned int Elevator::Id() const
{
	return myId;
}

std::string Elevator::ToString() const
{
	return "- State: " 
		+ std::to_string(myCurrentFloor) 
		+ "/" 
		+ std::to_string(myFloorCount) 
		+ " " 
		+ ::ToString(myCurrentDirection);
}

bool Elevator::CanResponseElevatorCall(const MessageElevatorCall& msg) const
{
    if (!HasWork())
	{
		return true;
	}
	if (msg.myDirection == myCurrentDirection)
	{
		switch (msg.myDirection)
		{
			case Direction::Up: return msg.myFloor >= myCurrentFloor;
			case Direction::Down: return msg.myFloor <= myCurrentFloor;
		}
	}
	return false;
}
