#pragma once

#include "../common/common.h"
#include "../common/module.h"

class Module_Game : public Module
{
public:
	Module_Game();
	virtual ~Module_Game();
	virtual int	setUp(IBus *bus);
	virtual int	input(const std::string& type, const void *data, IBus *bus);
	virtual int	tearDown(IBus *bus);
private:
};