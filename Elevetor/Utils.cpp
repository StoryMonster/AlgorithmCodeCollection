#include <chrono>
#include <random>

#include "Utils.h"

std::mutex locLogMutex;

int Random(const int aFrom,	const int aTo)
{
	static std::random_device device;
	static std::mt19937 gen(device());
    
	std::uniform_int_distribution<> dis(aFrom, aTo);

    return dis(gen);
}
