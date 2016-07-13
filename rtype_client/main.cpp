#include <vector>
#include <map>
#include <string>
#include "../common/common.h"
#include "../engine/Engine.h"

int	main(int ac, const char **av)
{
	(void)ac;
	(void)av;
#ifdef OS_WINDOWS
	std::vector<std::string> modules = {"module_network.dll", "module_config.dll", "module_tick.dll", "module_sfml.dll"};
#elif defined(OS_LINUX)
	std::vector<std::string> modules = {"../lib/libmodule_network.so", "../lib/libmodule_config.so", "../lib/libmodule_tick.so", "../lib/libmodule_sfml.so"};
#endif
	
	/*for (int i = 0; i < 15; i++)
	{
		std::string saude = "netwurk";
		modulestoStart[saude + std::to_string(i)] = "network";
	}*/
	Engine	*engine = new Engine(modules, 5);
	engine->getBus()->add("network", "network");
	engine->getBus()->add("tick", "tick");
	engine->getBus()->add("config", "config");
	engine->getBus()->add("sfml", "sfml");
	engine->start();
	Engine::Worker::run(engine);
	//std::this_thread::sleep_for(std::chrono::seconds(15));
	delete engine;
	return 0;
}
