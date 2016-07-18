#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <string>
#include <map>

class Module_Bydos1 : public Module
{
public:
	Module_Bydos1();
	virtual ~Module_Bydos1();
	virtual int	setUp(IBus *bus);
	virtual int	input(const std::string& type, const void *data, IBus *bus);
	virtual int	tearDown(IBus *bus);
private:
	std::string	name;
};