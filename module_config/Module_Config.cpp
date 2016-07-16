#include "Module_Config.h"
#include "local.h"
#include "../common/templatedTools.h"
#include "msg.h"
#include <sstream>
#include <fstream>
#include <iostream>


Module_Config::Module_Config()
{
}

Module_Config::~Module_Config()
{


}
int	Module_Config::setUp(IBus *bus)
{
	(void)bus;
	std::ifstream configFile("config.conf");
	if (configFile.good())
	{
		std::string line = "";
		while (std::getline(configFile, line))
		{
			if (line.length() > 0)
			{
				std::size_t cut = line.find_first_of('=');
				if (cut == std::string::npos)
				{
					configFile.close();
					return 1;
				}
				map[line.substr(0, cut)] = line.substr(cut + 1, line.length() - cut - 1);
			}
		}
		configFile.close();
	}
	else
	{
		map["ips"] = "60";
		map["serverip"] = "127.0.0.1";
	}
	for (auto it = map.begin(); it != map.end(); it++)
	{
		bus->in(MSG_CONFIG_RESULT, &*it, nullptr);	
	}
	return 0;
}
int	Module_Config::input(const std::string& type, const void *data, IBus *bus)
{
	
	if (type == MSG_CONFIG)
	{
		const std::string* sdata = (const std::string*)data;
		auto find = map.find(*sdata);
		if (find != map.end())
			bus->in(MSG_CONFIG_RESULT, &*find, nullptr);
	}
	return 0;
}
int	Module_Config::tearDown(IBus *bus)
{
	(void)bus;
	return 0;
}
