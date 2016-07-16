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
	std::vector<std::string> modules = {"module_network.dll", "module_config.dll", "module_tick.dll", "module_game.dll"};
#elif defined(OS_LINUX)
	std::vector<std::string> modules = {"./libmodule_network.so", "./libmodule_config.so", "./libmodule_tick.so", "./libmodule_game.so"};
#endif
	
	/*for (int i = 0; i < 15; i++)
	{
		std::string saude = "netwurk";
		modulestoStart[saude + std::to_string(i)] = "network";
	}*/
	Engine	*engine = new Engine(modules, 15);
	engine->getBus()->add("network", "network");
	engine->getBus()->add("tick", "tick");
	engine->getBus()->add("game", "game");
	engine->getBus()->add("config", "config");
	engine->start();
	Engine::Worker::run(engine);
	while (engine->getNbWorkersNotExited() > -1)
	{
		std::this_thread::sleep_for(WAIT_SLEEP_TIME);
	}
	delete engine;
	return 0;
}
