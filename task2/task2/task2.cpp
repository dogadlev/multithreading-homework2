#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <exception>
#include <Windows.h>

#define INDENT 31
#define TAB 4

std::mutex m0;

void GoToXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, coord);
}

void printBar(int serialNum, int calcDuration)
{
	auto start = std::chrono::high_resolution_clock::now();
	m0.lock();
	GoToXY(0, serialNum);
	std::cout << "Thread " << serialNum << " with thread id " << std::this_thread::get_id() << " ";
	m0.unlock();
	std::this_thread::yield();
	for (int progress = 0; progress <= calcDuration; ++progress)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		m0.lock();
		GoToXY((INDENT + progress), serialNum);
		std::cout << "||";
		m0.unlock();
		std::this_thread::yield();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1, 1000>> time = end - start;
	std::lock_guard<std::mutex> lg(m0);
	GoToXY((INDENT + calcDuration + TAB), serialNum);
	std::cout << time.count() << " ms";
}

int main(int argc, char** argv)
{
	try
	{
		int threadsQuant = 4, calcDuration = 30;
		if (threadsQuant <= 0 || calcDuration <= 0)
			throw std::runtime_error("Wrong inputs");
		else
		{
			std::vector<std::thread> threads;
			for (int i = 0; i < threadsQuant; ++i)
				threads.push_back(std::thread(printBar, i, calcDuration));
			for (auto& t : threads)
				t.join();
			
			for (int i = 0; i < threadsQuant; ++i)
				std::cout << std::endl;
		}
	}
	catch (std::runtime_error& e) { std::cout << e.what() << std::endl; }
	catch (...) { std::cout << "Something has gone wrong!" << std::endl; }

	return 0;
}