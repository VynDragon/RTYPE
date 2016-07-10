#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <thread>
#ifdef OS_LINUX
#include <ctime>
#elif defined(OS_WINDOWS)
#define CLOCK_REALTIME 0
#include <windows.h>
int clock_gettime(int, struct timespec *spec);
#endif


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