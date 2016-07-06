#include <vector>
#include <map>
#include <string>
#include "../common/common.h"
#include "../engine/Engine.h"

void testan()
{
	std::vector<std::string> modules = {"../module_network/libmodule_network.so"};
	std::map<std::string,std::string> modulestoStart;
	modulestoStart["network"] = "network";
	Engine	*engine = new Engine(modules, modulestoStart, 5);
	delete engine;
}

int	main(int ac, const char **av)
{
	(void)ac;
	(void)av;
	testan();
	//Engine::Worker::run(&engine);
	return 0;
}
