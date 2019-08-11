#include <chrono>
#include "Threads.h"

// Shrink this number to make the system go faster.
static const unsigned int locThreadWaitTimeMs = 1000;

void Threads::ElevatorWorker()
{
	std::vector<std::function<void()>> work;

	while(true)
	{
		{
			std::unique_lock<std::mutex> lock(myElevatorsMutex);
			myElevatorsCv.wait_for(lock, std::chrono::milliseconds(locThreadWaitTimeMs));

			work.swap(myElevatorsWork);
			myElevatorsWork.clear();
		}

		for(auto wrk : work)
		{
			wrk();
		}
	}
}

void Threads::HumansWorker()
{
	std::vector<std::function<void()>> work;

	while(true)
	{
		{
			std::unique_lock<std::mutex> lock(myHumansMutex);
			myHumansCv.wait_for(lock, std::chrono::milliseconds(locThreadWaitTimeMs));

			work.swap(myHumansWork);
			myHumansWork.clear();
		}

		for(auto wrk : work)
		{
			wrk();
		}
	}
}

void Threads::Start()
{
	myElevatorsThread = std::thread(&Threads::ElevatorWorker, this);
	myHumansThread = std::thread(&Threads::HumansWorker, this);
}

void Threads::Wait()
{
	myElevatorsThread.join();
	myHumansThread.join();
}

void Threads::AddElevatorWork(std::function<void()> aWork)
{
	{
		std::unique_lock<std::mutex> lock(myElevatorsMutex);
		myElevatorsWork.emplace_back(aWork);
	}

	myElevatorsCv.notify_all();
}

void Threads::AddHumanWork(std::function<void()> aWork)
{
	{
		std::unique_lock<std::mutex> lock(myHumansMutex);
		myHumansWork.emplace_back(aWork);
	}

	myHumansCv.notify_all();
}
