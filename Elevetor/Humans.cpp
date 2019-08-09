#include "MessageBus.h"

#include "Humans.h"

Human::Human(
	const unsigned int aFloor,
	const unsigned int aDestinationFloor)
	: myFloor(aFloor)
	, myDestinationFloor(aDestinationFloor)
	, myState(HumanState_Idle)
	, myWaitingCounter(0)
	, myTravelingCounter(0)
	, myElevatorId(0x7fffffffffffffff)
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
    if (myState == HumanState_Traveling && myElevatorId == msg.myElevatorId && myDestinationFloor == msg.myFloor)
	{
		SetStateArrived();
	}
	else if (myState == HumanState_Waiting && msg.myFloor == myFloor)
	{
		const auto myDirection = myDestinationFloor > myFloor ? Direction::Up : Direction::Down;
		if (myDirection == msg.myDirection)
		{
            myElevatorId = msg.myElevatorId;
		    SetStateTraveling();
		}
	}
}

///

Humans::Humans()
{
}

void Humans::Start()
{
	REGISTER_HUMAN(MessageElevatorReady, Humans::OnMessageElevatorReady);
	REGISTER_HUMAN(MessageElevatorArrived, Humans::OnMessageElevatorArrived);
	REGISTER_HUMAN(MessageHumanStep, Humans::OnMessageHumanStep);

	myHumans.push_back(Human(1, 4));
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
		if (prevState == HumanState_Traveling && nextState == HumanState_Arrived)
		{
			Log("[Humans] Human arrived destination, and leave the elevator");
		}
		else if (prevState == HumanState_Waiting && nextState == HumanState_Traveling)
		{
			Log("[Humans] Human enter the elevator");
			MessageElevatorRequest msg{aMessage.myElevatorId, human.myDestinationFloor};
			SEND_TO_ELEVATORS(msg);
		}
	}
 
}

void Humans::OnMessageHumanStep(const MessageHumanStep& aMessage)
{	
	Log("[Humans] Step");

	for (Human& human : myHumans)
	{
		human.Step();
	}

	PrivPrintTimers();

	// Implement me!
	for (auto& human : myHumans)
	{
		if (human.GetState() == HumanState::HumanState_Idle)
		{
			const auto humanDirection = human.myFloor < human.myDestinationFloor ? Direction::Up : Direction::Down;
			MessageElevatorCall msg{human.myFloor, humanDirection};
			SEND_TO_ELEVATORS(msg);
		    human.SetStateWaiting();
		}
	}

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
