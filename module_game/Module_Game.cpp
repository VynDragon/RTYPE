#include "Module_Game.h"
#include "../module_network/msg.h"
#include "../common/templatedTools.h"
#include <iostream>

Module_Game::Module_Game()
{
}

Module_Game::~Module_Game()
{


}
int	Module_Game::setUp(IBus *bus)
{
	bus->in(MSG_NETWORK_BIND, new int(3100), delFunction<int*>, "network");
	return 0;
}
int	Module_Game::input(const std::string& type, const void *data, IBus *bus)
{

	return 0;
}
int	Module_Game::tearDown(IBus *bus)
{
	return 0;
}
