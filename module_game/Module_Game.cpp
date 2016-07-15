#include "Module_Game.h"
#include "../module_network/msg.h"
#include "../module_config/msg.h"
#include "../common/templatedTools.h"
#include <iostream>
#include <tuple>

Module_Game::Module_Game()
{
}

Module_Game::~Module_Game()
{


}
int	Module_Game::setUp(IBus *bus)
{
#ifdef CLIENT
#else
	bus->in(MSG_NETWORK_BIND, new int(3100), delFunction<int*>, "network");
#endif
	return 0;
}
int	Module_Game::input(const std::string& type, const void *data, IBus *bus)
{
#ifdef CLIENT
	if (type == MSG_CONFIG_RESULT)
	{
		std::pair<std::string, std::string>* sdata = (std::pair<std::string, std::string>*)data;
		if (sdata->first == "serverip")
		{
			bus->in(MSG_NETWORK_ADDCLIENT, new std::tuple<std::string, std::string, int>("server", sdata->second, PORT), delFunction<std::tuple<std::string, std::string, int>*>, "network");
			bus->in(MSG_NETWORK_EXPORT, new std::tuple<std::string, std::string, size_t, const void*>("", "DOOT", 0, nullptr), delFunction<std::tuple<std::string, std::string, size_t, const void*>*>, "network");
		}
	}
#else
#endif
	return 0;
}
int	Module_Game::tearDown(IBus *bus)
{
	return 0;
}
