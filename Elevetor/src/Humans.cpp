#include "Humans.h"
#include <cassert>
#include "Utils.h"
#include "MessageBus.h"

Human::Human(
	const unsigned int aFloor,
	const unsigned int aDestinationFloor)
	: myFloor(aFloor)
	, myDestinationFloor(aDestinationFloor)
	, myState(HumanState_Idle)
	, myWaitingCounter(0)
	, myTravelingCounter(0)
	, myElevatorId(0x7fffffff)
{
	if (myFloor == myDestinationFloor)
	{
		Log("[Human][Error] Same floor and destination floor: ", myFloor);
		assert(false);
	}
}

HumanState Human::GetState() const
{
	return myState;
}

void Human::SetStateWaiting()
{
	myState = HumanState_Waiting;
}

void Human::SetStateTraveling()
{
	myState = HumanState_Traveling;
}

void Human::SetStateArrived()
{
	myState = HumanState_Arrived;
}

void Human::Step()
{
	if (myState == HumanState_Traveling)
	{
		++myTravelingCounter;
	}
	if (myState == HumanState_Waiting)
	{
		++myWaitingCounter;
	}
}

unsigned int Human::GetWaitingCounter() const
{
	return myWaitingCounter;
}

unsigned int Human::GetTravelingCounter() const
{
	return myTravelingCounter;
}

void Human::OnElevatorArrived(const MessageElevatorArrived& msg)
{
	if (msg.myType == ElevatorArriveType::ArrivedCallingFloor && myState == HumanState_Waiting && myFloor == msg.myFloor)
	{
		const auto myDirection = myDestinationFloor > myFloor ? Direction::Up : Direction::Down;
		if (myDirection == msg.myDirection)
		{
			myElevatorId = msg.myElevatorId;
		    SetStateTraveling();
		}
	}
	else if (msg.myType == ElevatorArriveType::ArrivedRequestFloor &&
	         myState == HumanState_Traveling &&
			 myDestinationFloor == msg.myFloor &&
			 myElevatorId == msg.myElevatorId)
	{
        SetStateArrived();
	}
}

///

Humans::Humans()
{
	REGISTER_HUMAN(MessageElevatorReady, Humans::OnMessageElevatorReady);
	REGISTER_HUMAN(MessageElevatorArrived, Humans::OnMessageElevatorArrived);
	REGISTER_HUMAN(MessageHumanStep, Humans::OnMessageHumanStep);
}

void Humans::Start()
{
	myHumans.push_back(Human(1, 4));
	myHumans.push_back(Human(2, 8));
}

void Humans::OnMessageElevatorReady(const MessageElevatorReady& aMessage)
{
	Log("[Humans] Elevator system ready");

	MessageHumanStep message;
	SEND_TO_HUMANS(message);
}

void Humans::OnMessageElevatorArrived(const MessageElevatorArrived&	aMessage)
{
	Log("[Humans] Elevator arrived at floor:", aMessage.myFloor);

	// Implement me!
	for (Human& human : myHumans)
	{
		const auto prevState = human.GetState();
		human.OnElevatorArrived(aMessage);
		const auto nextState = human.GetState();
		if (prevState == HumanState_Waiting && nextState == HumanState_Traveling)
		{
			MessageElevatorRequest msg{aMessage.myElevatorId, human.myDestinationFloor};
			SEND_TO_ELEVATORS(msg);
		}
	}
}

void Humans::OnMessageHumanStep(const MessageHumanStep& aMessage)
{	
	Log("[Humans] Step");
	// Implement me!

    for (auto& human : myHumans)
	{
		human.Step();
		if (human.GetState() == HumanState::HumanState_Idle)
		{
			const auto humanDirection = human.myDestinationFloor > human.myFloor ? Direction::Up : Direction::Down;
			MessageElevatorCall msg{human.myFloor, humanDirection};
			SEND_TO_ELEVATORS(msg);
		    human.SetStateWaiting();
		}
	}
	PrivPrintTimers();

	MessageHumanStep message;
	SEND_TO_HUMANS(message);
}

void Humans::PrivPrintTimers()
{
	unsigned int sumWaiting = 0;
	unsigned int sumTraveling = 0;
	unsigned int humansArrivedCounter = 0;

	for (const Human& human : myHumans)
	{
		if (human.GetState() != HumanState_Arrived)
		{
			continue;
		}

		sumWaiting += human.GetWaitingCounter();
		sumTraveling += human.GetTravelingCounter();
		++humansArrivedCounter;
	}

	if (humansArrivedCounter == 0)
	{
		return;
	}

	Log("[Stats] Average wait time (steps): ", (sumWaiting / static_cast<float>(humansArrivedCounter)));
	Log("[Stats] Average travel time (steps): ", (sumTraveling / static_cast<float>(humansArrivedCounter)));
}
