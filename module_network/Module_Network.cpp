#include "Module_Network.h"
#include <iostream>

Module_Network::Module_Network()
{
}

Module_Network::~Module_Network()
{


}
int	Module_Network::setUp(IBus *bus)
{
	std::cout << "setUp" << std::endl;
	return 0;
}
int	Module_Network::input(const std::string& type, const void *data, IBus *bus)
{
	std::cout << "input" << std::endl;
	return 0;
}
int	Module_Network::tearDown(IBus *bus)
{
	std::cout << "tearDown" << std::endl;
	return 0;
}
