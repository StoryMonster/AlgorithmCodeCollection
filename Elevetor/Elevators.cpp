#include "MessageBus.h"

#include "Elevators.h"


namespace
{

unsigned int abs(unsigned int a, unsigned int b)
{
	return a > b ? a - b : b - a;
}
}

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
	unsigned int minDistance = 0x7fffffffffffffff;
	auto elevator = myElevators.end();
	for (auto it = myElevators.begin(); it != myElevators.end(); ++it)
	{
		if (it->CanResponseElevatorCall(aMessage))
		{
			unsigned int distance = abs(aMessage.myFloor, it->CurrentFloor());
			if (minDistance > distance)
		    {
			    minDistance = distance;
			    elevator = it;
		    }
		}
	}
	if (elevator == myElevators.end())
	{
		unsolvedCalls.push(aMessage);
	}
	else
	{
		elevator->SelectFloor(aMessage.myFloor);
	}
}



void Elevators::OnMessageElevatorRequest(const MessageElevatorRequest& aMessage)
{
	// Implement me!
	for (auto& elevator : myElevators)
	{
		if (elevator.Id() != aMessage.myElevatorId) { continue; }
		elevator.SelectFloor(aMessage.myFloor);
		break;
	}
}

void  Elevators::OnMessageElevatorStep(const MessageElevatorStep& aMessage)
{
	Log("[Elevators] Step");

	for (auto& elevator : myElevators)
	{
		elevator.Step();
		if (elevator.WillStopAtCurrentFloor())
		{
			elevator.StopAtCurrentFloor();
			MessageElevatorArrived msg{elevator.Id(), elevator.CurrentFloor(), elevator.CurrentDirection()};
			SEND_TO_HUMANS(msg);
		}
	}

	for (int i = 0; i < unsolvedCalls.size(); ++i)
	{
		const auto call = unsolvedCalls.front();
		unsolvedCalls.pop();
		OnMessageElevatorCall(call);
	}

	MessageElevatorStep message;
	SEND_TO_ELEVATORS(message);
}
