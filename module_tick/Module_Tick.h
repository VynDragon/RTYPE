#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <thread>
#ifdef OS_LINUX
#include <ctime>
#elif defined(OS_WINDOWS)
#include <windows.h>
struct timespec { long tv_sec; long tv_nsec; };    //header part
int clock_gettime(int, struct timespec *spec)      //C-file part
{  __int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
   wintime      -=116444736000000000i64;  //1jan1601 to 1jan1970
   spec->tv_sec  =wintime / 10000000i64;           //seconds
   spec->tv_nsec =wintime % 10000000i64 *100;      //nano-seconds
   return 0;
}
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
	uint		fps = 60;
	timespec	lastTick;
};