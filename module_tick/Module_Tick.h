#pragma once

#include "../common/common.h"
#include "../common/module.h"
#ifdef OS_LINUX
#include <ctime>
#elif defined(OS_WINDOWS)
#define CLOCK_REALTIME 0
#include <windows.h>
//struct timespec { long tv_sec; long tv_nsec; };
int clock_gettime(int, struct timespec *spec);
#endif
#include <thread>


class Module_Tick : public Module
{
public:
	Module_Tick();
	virtual ~Module_Tick();
	virtual int	setUp(IBus *bus);
	virtual int	input(const std::string& type, const void *data, IBus *bus);
	virtual int	tearDown(IBus *bus);
private:
	unsigned int	ips = 60;
	timespec		lastTick;
};