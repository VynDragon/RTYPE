#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <string>
#include <map>

class Module_Config : public Module
{
public:
	Module_Config();
	virtual ~Module_Config();
	virtual int	setUp(IBus *bus);
	virtual int	input(const std::string& type, const void *data, IBus *bus);
	virtual int	tearDown(IBus *bus);
private:
	std::map<std::string, std::string>	map;
};