#include <condition_variable>
#include <mutex>
#include "Elevators.h"
#include "Humans.h"
#include "MessageBus.h"
#include "Threads.h"

Elevators elevators;
Humans humans;

void RunElevators()
{
	elevators.Start();
}

void RunHumans()
{
	humans.Start();
}

int main()
{
	Threads::GetInstance().Start();
	MessageBus::GetInstance();

	Threads::GetInstance().AddElevatorWork(std::bind(&RunElevators));
	Threads::GetInstance().AddHumanWork(std::bind(&RunHumans));

	Threads::GetInstance().Wait();
	return 0;
}
