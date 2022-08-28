#include <thread>
#include <iostream>
#include <chrono>
#include <vector>

#include "cmdline.h"

#include <Windows.h>
using namespace std;

void loading(unsigned per,unsigned core)
{
	chrono::high_resolution_clock::time_point start, end;

	for (;;)
	{
		int64_t time = 0;
		start = chrono::high_resolution_clock::now();
		while (time < per )
		{
			end = chrono::high_resolution_clock::now();
			time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
		}
		std::this_thread::sleep_for(chrono::milliseconds(100- time));
	}
}

int main(int argc, char* argv[])
{
	int max_cores = std::thread::hardware_concurrency();

	cmdline::parser a;
	a.add<int>("usage", 'u', "cpu usage", false, 45, cmdline::range(1, 100));
	a.add<int>("cores", 'c', "cores", false, 1, cmdline::range(1, max_cores));
	a.parse_check(argc, argv);

	auto usage = a.get<int>("usage");
	auto core_num = a.get<int>("cores");


	std::vector<thread> worker;
	for (unsigned i =0;i< core_num;++i)
	{
		uint64_t mask = 0x1;
		std::thread t=thread(loading, (usage/4)*3, i);
		SetThreadAffinityMask(t.native_handle(), mask << i);
		worker.push_back(std::move(t));
	}

	for (auto& v : worker)
	{
		v.join();
	}

	return 0;
}