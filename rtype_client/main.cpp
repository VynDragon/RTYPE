#include <vector>
#include <map>
#include <string>
#include "../common/common.h"
#include "../engine/Engine.h"

int	main(int ac, const char **av)
{
	(void)ac;
	(void)av;
	std::vector<std::string> modules = {"../module_network/libmodule_network.so"};
	std::map<std::string,std::string> modulestoStart;
	modulestoStart["network"] = "network";
	for (int i = 0; i < 15; i++)
	{
		std::string saude = "netwurk";
		modulestoStart[saude + std::to_string(i)] = "network";
	}
	Engine	*engine = new Engine(modules, modulestoStart, 5);
	engine->start();
	//Engine::Worker::run(engine);
	std::this_thread::sleep_for(std::chrono::seconds(15));
	delete engine;
	return 0;
}
