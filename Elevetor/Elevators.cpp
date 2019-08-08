#include "MessageBus.h"

#include "Elevators.h"

Elevators::Elevators()
{
}

void Elevators::Start()
{
	REGISTER_ELEVATOR(MessageElevatorCall, Elevators::OnMessageElevatorCall);
	REGISTER_ELEVATOR(MessageElevatorStep, Elevators::OnMessageElevatorStep);
	REGISTER_ELEVATOR(MessageElevatorRequest, Elevators::OnMessageElevatorRequest);

	myElevators.push_back(Elevator{1, 10, 6, Direction::Down});

	{
		MessageElevatorStep message;
		SEND_TO_ELEVATORS(message);
	}

	{
		MessageElevatorReady message;
		SEND_TO_HUMANS(message);
	}
}

void Elevators::OnMessageElevatorCall(const MessageElevatorCall& aMessage)
{
	// Implement me!
}

void Elevators::OnMessageElevatorRequest(const MessageElevatorRequest& aMessage)
{
	// Implement me!
}

void
Elevators::OnMessageElevatorStep(const MessageElevatorStep&	aMessage)
{
	Log("[Elevators] Step");

	for (auto& elevator : myElevators)
	{
		elevator.Step();
	}

	MessageElevatorStep message;
	SEND_TO_ELEVATORS(message);
}