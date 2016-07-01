#pragma once

#include "../common/common.h"
#include "../common/module.h"

class Module_Network : public Module
{
public:
	Module_Network();
	~Module_Network();
	int	in(const std::string& type, const void *data);
private:
};