#include <thread>
#include <iostream>
#include <chrono>
#include <vector>

#include "cmdline.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <stdio.h>
#define __USE_GNU
#include <sched.h>
#include <pthread.h>

bool SetThreadAffinityMask(std::thread::native_handle_type handle, uint64_t mask)
{
	int result = 0;
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	unsigned core=0;
	while(mask!=1)
	{
		mask = mask>>1;
		core++;
	}
	CPU_SET(core, &cpuset);
	result = pthread_setaffinity_np(
		handle,
		sizeof(cpu_set_t),
		&cpuset);
	return result == 0;
}
#endif // __WIN32


using namespace std;

static bool stop = false;

void loading(unsigned per)
{
	chrono::high_resolution_clock::time_point start, end;

	while(!stop)
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

std::vector<int> parseMask(string mask)
{
	std::vector<int> cores;
	unsigned int mask_pos = 1;
	unsigned count = 0;
	for (int i = 0; i < mask.size();++i)
	{
		if (mask.at(i) == '1')
		{
			mask_pos = 1 <<(mask.size()-1- i);
			cores.push_back(mask_pos);

			++count;
		}
	}
	return cores;
}

bool isMaskLegal(string mask)
{
	auto max_cores = std::thread::hardware_concurrency();
	if (mask.size() > max_cores)
	{
		std::cerr << "option value is invalid: --mask=" << mask << endl;
		std::cerr << "The mask length is greater than the maximum number of cores";
		return false;
	}

	for (auto& c : mask)
	{
		if (c != '1' && c != '0')
		{
			std::cerr << "option value is invalid: --mask=" << mask << endl;
			std::cerr << "Please enter a binary number. e.g. 11001001";
			return false;
		}
	}

	return true;
}

int main(int argc, char* argv[])
{
	int max_cores = std::thread::hardware_concurrency();

	cmdline::parser a;
	a.add<int>("usage", 'u', "cpu usage", false, 45, cmdline::range(1, 100));
	a.add<int>("cores", 'c', "if -m(mask) is set, this parameter will be ignored", false, 1, cmdline::range(1, max_cores));
	a.add<string>("mask", 'm', "specify cpu cores by mask",false);
	a.parse_check(argc, argv);

	auto usage = a.get<int>("usage");
	std::vector<thread> worker;

	if (a.exist("mask"))
	{
		auto mask_str = a.get<string>("mask");
		if (!isMaskLegal(mask_str))
			return -1;

		auto core_by_mask = parseMask(mask_str);

		for (auto& core: core_by_mask)
		{
			uint64_t mask = 0x1;
			std::thread t = thread(loading, usage);
			auto ret = SetThreadAffinityMask(t.native_handle(), core);
			worker.push_back(std::move(t));
			if (ret == 0)
			{
				stop = true;
				std::cerr << "Set thread affinity mask failed at Core[" << core << "]";
			}
		}
	}
	else
	{
		auto core_num = a.get<int>("cores");
		for (unsigned i = 0; i < core_num; ++i)
		{
			uint64_t mask = 0x1;
			std::thread t = thread(loading, usage);
			auto ret = SetThreadAffinityMask(t.native_handle(), mask << i);
			worker.push_back(std::move(t));
			if (ret == 0)
			{
				stop = true;
				std::cerr << "Set thread affinity mask failed at Core[" << i << "]";
			}
		}
	}


	for (auto& v : worker)
	{
		v.join();
	}
	return 0;
}