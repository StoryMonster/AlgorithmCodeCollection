#pragma once

#include <mutex>
#include <vector>
#include "Elevator.h"
#include "Messages.h"

class Elevators
{
public:
	Elevators();

	void Start();

	void OnMessageElevatorCall(const MessageElevatorCall& aMessage);
	void OnMessageElevatorRequest(const MessageElevatorRequest& aMessage);
	void OnMessageElevatorStep(const MessageElevatorStep& aMessage);

private:
	std::vector<Elevator> myElevators;
};
