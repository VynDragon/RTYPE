#include "Module_Bydos1.h"
#include "local.h"
#include "../common/templatedTools.h"
#include "msg.h"
#include <iostream>


Module_Bydos1::Module_Config()
{
}

Module_Bydos1::~Module_Config()
{


}
int	Module_Bydos1::setUp(IBus *bus)
{
	return 0;
}
int	Module_Bydos1::input(const std::string& type, const void *data, IBus *bus)
{

	return 0;
}
int	Module_Bydos1::tearDown(IBus *bus)
{
	(void)bus;
	return 0;
}
