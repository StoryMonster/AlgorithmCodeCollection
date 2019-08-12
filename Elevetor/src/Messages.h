#pragma once

#include <string>
#include <tuple>

enum Direction
{
	Up = 0,
	Down
};

enum ElevatorArriveType
{
	ArrivedCallingFloor = 0,
	ArrivedRequestFloor
};

class MessageElevatorReady
{
};

class MessageElevatorStep
{
};

class MessageElevatorCall
{
public:
	unsigned int myFloor;
	Direction myDirection;

	bool operator<(const MessageElevatorCall& msg) const
	{
		return std::tie(myFloor, myDirection) < std::tie(msg.myFloor, msg.myDirection);
	}
};

class MessageElevatorRequest
{
public:
	unsigned int myElevatorId;
	unsigned int myFloor;
};

class MessageElevatorArrived
{
public:
	unsigned int myElevatorId;
	unsigned int myFloor;
	Direction myDirection;
	ElevatorArriveType myType;
};

class MessageHumanStep
{
};

static std::string ToString(const Direction aDirection)
{
	switch (aDirection)
	{
	case Direction::Up:
		return "Up";
	case Direction::Down:
		return "Down";
	default:
		return "Unknown";
		break;
	}
}
