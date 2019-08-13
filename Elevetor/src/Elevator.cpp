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
}

void Elevator::SelectFloor(const unsigned int aFloorId)
{
	// Implement me!
    requestFloors.emplace(aFloorId);
}

void Elevator::RespondToCall(const MessageElevatorCall& msg)
{
	if (!HasWork())
	{
		myCurrentDirection = msg.myFloor > myCurrentFloor ? Direction::Up : Direction::Down;
	}
    calls.emplace(msg);
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
	return !calls.empty() || !requestFloors.empty();
}

void Elevator::Step()
{
	// Implement me!
	if (!HasWork()) { return; }
	Log(ToString());
	if (myCurrentDirection == Direction::Down)
	{
		--myCurrentFloor;
		if (myCurrentFloor < 1)
		{
			myCurrentDirection = Direction::Up;
            myCurrentFloor = 1;
		}
	}
	else
	{
		++myCurrentFloor;
		if (myCurrentFloor > myFloorCount)
		{
			myCurrentDirection = Direction::Down;
			myCurrentFloor = myFloorCount;
		}
	}
}

bool Elevator::IsStoppableCallingFloor() const
{
	if (calls.empty()) { return false; }
	const auto call = calls.cbegin();
	if (calls.size() == 1) { return call->myFloor == myCurrentFloor; }
	unsigned int minCallFloor = 0x7fffffff;
	unsigned int maxCallFloor = 0;
	for (auto it = calls.cbegin(); it != calls.cend(); ++it)
	{
        if (it->myFloor < minCallFloor) { minCallFloor = it->myFloor; }
		if (it->myFloor > maxCallFloor) { maxCallFloor = it->myFloor; }
	}
	if (myCurrentFloor != minCallFloor && myCurrentFloor != maxCallFloor) { return false; }
	if (myCurrentFloor == minCallFloor && myCurrentFloor == maxCallFloor) { return true; }
	if (myCurrentFloor == maxCallFloor)
	{
        return (call->myDirection == myCurrentDirection && myCurrentDirection == Direction::Down) ||
		       (call->myDirection != myCurrentDirection && myCurrentDirection == Direction::Up);
	}
	else
	{
		return (call->myDirection == myCurrentDirection && myCurrentDirection == Direction::Up) ||
		       (call->myDirection != myCurrentDirection && myCurrentDirection == Direction::Down);
	}
}

bool Elevator::IsRequestFloor() const
{
	return requestFloors.find(myCurrentFloor) != requestFloors.cend();
}

void Elevator::HandleArrivedAtRequestFloor()
{
	if (IsRequestFloor())
	{
        requestFloors.erase(myCurrentFloor);
	}
}

void Elevator::HandleArrivedAtCallingFloor()
{
    const auto it = std::find_if(calls.cbegin(), calls.cend(), [this](const auto& msg){ return msg.myFloor == this->myCurrentFloor; });
	if (it != calls.cend())
	{
		myCurrentDirection = it->myDirection;
        calls.erase(it);
	}
}

unsigned int Elevator::Id() const
{
	return myId;
}

std::string Elevator::ToString() const
{
	return "[Elevator" + std::to_string(myId)
	    + "]- State: "
		+ std::to_string(myCurrentFloor) 
		+ "/" 
		+ std::to_string(myFloorCount) 
		+ " "
		+ ::ToString(myCurrentDirection);
}

bool Elevator::CanRespondToCall(const MessageElevatorCall& msg) const
{
    if (!HasWork())
	{
		return true;
	}
	const auto it = calls.cbegin();
	if (it != calls.cend())
	{
		if (msg.myDirection != it->myDirection) { return false; }
	}
	else
	{
		if (myCurrentDirection != msg.myDirection) { return false; }
	}
	return myCurrentDirection == Direction::Up ?  (msg.myFloor >= myCurrentFloor) : (msg.myFloor <= myCurrentFloor);
}
