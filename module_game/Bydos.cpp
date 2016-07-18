#include "Module_Game.h"
#include "../module_bydos1/msg.h"
#include "../common/templatedTools.h"
#include <ctime>

Bydos::Bydos(const std::string& networkId, IBus *bus) : Drawable(networkId, "Bydos1.png")
{
	this->rand.seed(time(nullptr));
	this->bus = bus;
	if(!bus->loaded("bydos1"))
#ifdef OS_WINDOWS
		bus->addModule("module_bydos1.dll");
#elif defined(OS_LINUX)
		bus->addModule("./libmodule_bydos1.so");
#endif
	bus->add(networkId, "bydos1");
	bus->in(MSG_AI_SETNAME, new std::string(networkId), delFunction<std::string*>, networkId);
	float div = (float)(rand.max() - rand.min());
	this->setColor((int)(((float)rand() / div) * 255.0f), (int)(((float)rand() / div) * 255.0f), (int)(((float)rand() / div) * 255.0f), 255);
	this->setXY(1.1f, (float)rand() / div);
}

Bydos::~Bydos()
{
	bus->remove(networkId);
}