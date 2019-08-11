#pragma once

#include <string>

enum Direction
{
	Up = 0,
	Down
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
