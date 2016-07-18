#include "Module_Bydos1.h"
#include "local.h"
#include "../common/templatedTools.h"
#include "../common/messages.h"
#include "msg.h"
#include <iostream>
#include <tuple>


Module_Bydos1::Module_Bydos1()
{
}

Module_Bydos1::~Module_Bydos1()
{


}
int	Module_Bydos1::setUp(IBus *bus)
{
	(void)bus;
	return 0;
}
int	Module_Bydos1::input(const std::string& type, const void *data, IBus *bus)
{
	if (type == MSG_AI_SETNAME)
	{
		std::string *sdata = (std::string*)data;
		this->name = *sdata;
	}
	else if (type == MSG_TICK)
	{
		bus->in(MSG_AI_MOVE, new std::tuple<std::string, float, float>(this->name, -0.005, 0), delFunction<std::tuple<std::string, float, float>*>);
	}
	return 0;
}
int	Module_Bydos1::tearDown(IBus *bus)
{
	(void)bus;
	return 0;
}
