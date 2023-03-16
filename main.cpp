#include <iostream>
#ifdef _WIN32
#include "windows.h"
#endif

#include <thread>
#include <mutex>


int count = 0;
std::mutex m;

void simulateClients(int clientCount)
{
	for (auto i = 0; i < clientCount; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		// https://en.cppreference.com/w/cpp/thread/lock_guard
		const std::lock_guard<std::mutex> lock(m);
		auto currCnt = ++count;
		std::cout << "Количество клиентов: " << currCnt << " (один пришел)" << std::endl;
	}
}

void simulateOperators()
{
	static bool planningToGo = false;

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		const std::lock_guard<std::mutex> lock(m);
		auto currCnt = count;
		
		if (currCnt <= 0)
		{
			std::cout << "Нет ни одного клиента, операционист уходит" << std::endl;
			break;
		}
		else
		{
			currCnt = --count;
			std::cout << "Количество клиентов: " << currCnt << " (один был обслужен)" << std::endl;
		}
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif

	auto clientCount = 10;
	std::cout << "Начало обслуживания клиентов, максимальное количество: " << clientCount << std::endl;

	std::thread clientThread(simulateClients, clientCount);
	std::thread operatorThread(simulateOperators);
	clientThread.join();
	operatorThread.join();
}