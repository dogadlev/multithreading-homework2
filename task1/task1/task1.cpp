#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>


std::atomic<unsigned int> clientsQty = 0;

void enlargeQueue()
{
	while (clientsQty != 20)
	{
		std::cout << (clientsQty.fetch_add(1, std::memory_order_relaxed) + 1) << " ";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void reduceQueue()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	while (clientsQty != 0)
	{
		std::cout << (clientsQty.fetch_sub(1, std::memory_order_relaxed) - 1) << " ";
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

int main(int argc, char** argv)
{
	std::thread t1(enlargeQueue);
	std::thread t2(reduceQueue);
	t1.join();
	t2.join();

	return 0;
}