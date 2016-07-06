#include "../common/common.h"
#include "../common/bus.h"

int	main(int ac, const char **av)
{
	Bus *bus = new Bus();
	bus->addModule("../module_network/libmodule_network.so");
	bus->add("network", "network");
	//bus->add("network", "network");
	return 0;
}
