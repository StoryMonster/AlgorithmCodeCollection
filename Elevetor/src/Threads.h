#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class Threads
{
public:
    static Threads& GetInstance()
	{
		static Threads instance;
		return instance;
	}

	void Start();
	void AddElevatorWork(std::function<void()> aWork);
	void AddHumanWork(std::function<void()>	aWork);
	void Wait();

private:
	Threads(){};
	void ElevatorWorker();

	std::thread myElevatorsThread;
	std::vector<std::function<void()>> myElevatorsWork;
	std::condition_variable myElevatorsCv;
	std::mutex myElevatorsMutex;

	void HumansWorker();

	std::thread myHumansThread;
	std::vector<std::function<void()>> myHumansWork;
	std::condition_variable myHumansCv;
	std::mutex myHumansMutex;
};
