#pragma once

#include <functional>
#include <vector>
#include "Threads.h"

class MessageBus
{
public:
	static MessageBus& GetInstance()
	{
		static MessageBus instance;
		return instance;
	}

	template<typename T> void SendToElevators(const T& aMessage);
	template<typename T> void SendToHumans(const T& aMessage);
	template<typename T> void RegisterElevator(const std::function<void(T)>& aCallback);
	template<typename T> void RegisterHuman(const std::function<void(T)>& aCallback);

private:
	MessageBus(){};
	template<typename T> std::vector<std::function<void(T)>>& PrivGetElevatorsCallbacks();
	template<typename T> std::vector<std::function<void(T)>>& PrivGetHumansCallbacks();
};

template<typename T>
void MessageBus::SendToElevators(const T& aMessage)
{
	auto& callbacks = PrivGetElevatorsCallbacks<T>();
	for (auto& callback : callbacks)
	{
		Threads::GetInstance().AddElevatorWork([=](){ callback(aMessage); });
	}
}

template<typename T>
void MessageBus::SendToHumans(const T& aMessage)
{
	auto& callbacks = PrivGetHumansCallbacks<T>();
	for (auto& callback : callbacks)
	{
		Threads::GetInstance().AddHumanWork([=](){ callback(aMessage); });
	}
}

template<typename T>
void MessageBus::RegisterElevator(const std::function<void(T)>& aCallback)
{
	auto& callbacks = PrivGetElevatorsCallbacks<T>();
	callbacks.push_back(aCallback);
}

template<typename T>
void MessageBus::RegisterHuman(const std::function<void(T)>& aCallback)
{
	auto& callbacks = PrivGetHumansCallbacks<T>();
	callbacks.push_back(aCallback);
}

template<typename T>
std::vector<std::function<void(T)>>& MessageBus::PrivGetElevatorsCallbacks()
{
	static std::vector<std::function<void(T)>> callbacks;
	return callbacks;
}

template<typename T>
std::vector<std::function<void(T)>>& MessageBus::PrivGetHumansCallbacks()
{
	static std::vector<std::function<void(T)>> callbacks;
	return callbacks;
}

#define REGISTER_ELEVATOR(MessageType, Func) \
{ std::function<void(MessageType)> callback = std::bind(&Func, this, std::placeholders::_1); \
MessageBus::GetInstance().RegisterElevator(callback); }

#define REGISTER_HUMAN(MessageType, Func) \
{ std::function<void(MessageType)> callback = std::bind(&Func, this, std::placeholders::_1); \
MessageBus::GetInstance().RegisterHuman(callback); }

#define SEND_TO_ELEVATORS(Message) \
MessageBus::GetInstance().SendToElevators(Message);

#define SEND_TO_HUMANS(Message) \
MessageBus::GetInstance().SendToHumans(Message);
