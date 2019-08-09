#include "Elevator.h"
#include "MessageBus.h"

#include "Elevators.h"

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
	if (aFloorId == myCurrentFloor) { return ;}
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
	//return false;
}

void Elevator::Step()
{
	// Implement me!
	if (myCurrentDirection == Direction::Down)
	{
		--myCurrentFloor;
		if (myCurrentFloor == 1)
		{
			myCurrentDirection = Direction::Up;
		}
	}
	else
	{
		++myCurrentFloor;
		if (myCurrentFloor == myFloorCount)
		{
			myCurrentDirection = Direction::Down;
		}
	}
}

bool Elevator::WillStopAtCurrentFloor() const
{
    return floorsNeedStop.find(myCurrentFloor) != floorsNeedStop.cend();
}

void Elevator::StopAtCurrentFloor() const
{
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
