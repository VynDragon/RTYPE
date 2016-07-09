#include "Module_Network.h"
#include "../module_config/local.h"
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
	bus->in("smes", nullptr, nullptr);
	return 0;
}
int	Module_Network::input(const std::string& type, const void *data, IBus *bus)
{
	count++;
	std::cout << "input: " << type << std::endl;
	if (data == nullptr)
	{
		std::cout << "data is null " << std::endl;
	}
	/*int kek = 0;
	while (kek < 1000000) { kek++;}
	for (int i = 0; i < 1; i++)
	{
		if (count < 100)
			bus->in("smes", nullptr, nullptr);
	}*/
	if (type == MSG_CONFIG_RESULT)
	{
		std::pair<std::string, std::string>* sdata  = (std::pair<std::string, std::string>*)data; 
		//std::cout << static_cast<void*>(&((*sdata).first)) << std::endl;
		std::cout << (*sdata).first << std::endl;
	}
	return 0;
}
int	Module_Network::tearDown(IBus *bus)
{
	std::cout << "tearDown" << std::endl;
	return 0;
}
