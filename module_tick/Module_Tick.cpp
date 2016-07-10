#include "Module_Tick.h"
#include "../module_config/msg.h"
#include "msg.h"
#include <iostream>
#include <chrono>

Module_Tick::Module_Tick()
{
}

Module_Tick::~Module_Tick()
{


}
int	Module_Tick::setUp(IBus *bus)
{
	clock_gettime(CLOCK_REALTIME, &lastTick);
	bus->in(MSG_TICK, nullptr, nullptr);
	return 0;
}
int	Module_Tick::input(const std::string& type, const void *data, IBus *bus)
{
	if (type == MSG_CONFIG_RESULT)
	{
		std::pair<std::string, std::string>* sdata = (std::pair<std::string, std::string>*)data;
		if (sdata->first == "ips")
		{
			this->ips = std::stoi(sdata->second);
			if (this->ips <= 0)
			{
				this->ips = 60;
			}
		}
	}
	else if (type == MSG_TICK)
	{
		timespec time;
		clock_gettime(CLOCK_REALTIME, &time);
		double actual_time = (time.tv_sec - lastTick.tv_sec) + (time.tv_nsec - lastTick.tv_nsec) / 1000000000.0;
		while (actual_time < 1.0 / (double)this->ips)
		{
			std::this_thread::sleep_for(WAIT_SLEEP_TIME);
			clock_gettime(CLOCK_REALTIME, &time);
			actual_time = (time.tv_sec - lastTick.tv_sec) + (time.tv_nsec - lastTick.tv_nsec) / 1000000000.0;
		}
		bus->in(MSG_TICK, nullptr, nullptr);
		clock_gettime(CLOCK_REALTIME, &lastTick);
	}
	
	return 0;
}
int	Module_Tick::tearDown(IBus *bus)
{
	(void)bus;
	return 0;
}
