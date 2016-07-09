#pragma once

#include "../common/common.h"
#include "../common/module.h"

class Module_Network : public Module
{
public:
	Module_Network();
	virtual ~Module_Network();
	virtual int	setUp(IBus *bus);
	virtual int	input(const std::string& type, const void *data, IBus *bus);
	virtual int	tearDown(IBus *bus);
private:
	int count = 0;
};