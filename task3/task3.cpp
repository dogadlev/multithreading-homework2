#include <iostream>
#include <mutex>


std::mutex m0;
std::mutex m1;

void swap_mutex_lock(std::mutex& m0, std::mutex& m1)
{
	std::unique_lock<std::mutex> ul0{ m0, std::defer_lock };
	std::unique_lock<std::mutex> ul1{ m1, std::defer_lock };
	lock(m0, m1);
	ul0.swap(ul1);
}

void swap_mutex_scoped_lock(std::mutex& m0, std::mutex& m1)
{
	std::unique_lock<std::mutex> ul0{ m0, std::defer_lock };
	std::unique_lock<std::mutex> ul1{ m1, std::defer_lock };
	std::scoped_lock sl{m0, m1};
	ul0.swap(ul1);
}

void swap_mutex_unique_lock(std::mutex& m0, std::mutex& m1)
{
	std::unique_lock<std::mutex> ul0{ m0, std::adopt_lock };
	std::unique_lock<std::mutex> ul1{ m1, std::adopt_lock };
	ul0.swap(ul1);
}

int main(int argc, char** argv)
{
	return 0;
}