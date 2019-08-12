#include "MessageBus.h"
#include "Elevators.h"
#include "Utils.h"

Elevators::Elevators()
{
	REGISTER_ELEVATOR(MessageElevatorCall, Elevators::OnMessageElevatorCall);
	REGISTER_ELEVATOR(MessageElevatorStep, Elevators::OnMessageElevatorStep);
	REGISTER_ELEVATOR(MessageElevatorRequest, Elevators::OnMessageElevatorRequest);
}

void Elevators::Start()
{
	myElevators.push_back(Elevator{1, 10, 4, Direction::Down});
	myElevators.push_back(Elevator{2, 10, 5, Direction::Down});

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
	unsigned int minDistance = 0x7fffffff;
	auto elevator = myElevators.end();
	auto abs = [](unsigned int a, unsigned int b) { return a > b ? a - b : b - a; };
	for (auto it = myElevators.begin(); it != myElevators.end(); ++it)
	{
		if (it->CanRespondToCall(aMessage))
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
		elevator->RespondToCall(aMessage);
	}
}



void Elevators::OnMessageElevatorRequest(const MessageElevatorRequest& aMessage)
{
	// Implement me!
	for (auto& elevator : myElevators)
	{
		if (elevator.Id() == aMessage.myElevatorId)
		{
			Log("Received request to ");
			elevator.SelectFloor(aMessage.myFloor);
		    break;
		}
	}
}

void  Elevators::OnMessageElevatorStep(const MessageElevatorStep& aMessage)
{
	Log("[Elevators] Step");

	for (auto& elevator : myElevators)
	{
		if (elevator.IsStoppableCallingFloor())
		{
			elevator.HandleArrivedAtCallingFloor();
			MessageElevatorArrived msg{elevator.Id(), elevator.CurrentFloor(), elevator.CurrentDirection(), ElevatorArriveType::ArrivedCallingFloor};
			SEND_TO_HUMANS(msg);
		}
		if (elevator.IsRequestFloor())
		{
			elevator.HandleArrivedAtRequestFloor();
			MessageElevatorArrived msg{elevator.Id(), elevator.CurrentFloor(), elevator.CurrentDirection(), ElevatorArriveType::ArrivedRequestFloor};
			SEND_TO_HUMANS(msg);
		}
		elevator.Step();
	}

	for (unsigned int i = 0; i < unsolvedCalls.size(); ++i)
	{
		const auto call = unsolvedCalls.front();
		unsolvedCalls.pop();
		OnMessageElevatorCall(call);
	}

	MessageElevatorStep message;
	SEND_TO_ELEVATORS(message);
}
