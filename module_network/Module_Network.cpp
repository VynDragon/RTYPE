#include "Module_Network.h"
#include "../module_config/msg.h"
#include "msg.h"
#include <iostream>
#ifdef OS_LINUX
#include <unistd.h>
#include <netinet/in.h>
#endif

const std::map<std::string, tfunctionType> Module_Network::tfunctions = {
	{MSG_NETWORK_EXPORT, &Module_Network::send_msg},
	{MSG_NETWORK_EXPORT_ACK, &Module_Network::send_msg_ack},
	{MSG_NETWORK_BIND, &Module_Network::bind}
};


Module_Network::Module_Network()
{
}

Module_Network::~Module_Network()
{


}
int	Module_Network::setUp(IBus *bus)
{
	this->socket = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (this->socket == -1)
		return errno;
	
	return 0;
}
int	Module_Network::input(const std::string& type, const void *data, IBus *bus)
{

	return 0;
}
int	Module_Network::tearDown(IBus *bus)
{
	close(this->socket);
	return 0;
}

int	Module_Network::send_msg(const void *data, IBus *bus)
{
	return 0;	
}

int	Module_Network::send_msg_ack(const void *data, IBus *bus)
{
	return 0;	
}

int	Module_Network::bind(const void *data, IBus *bus)
{
	sockaddr_in in = {};
	in.sin_addr.s_addr = htonl(INADDR_ANY);
	in.sin_family = AF_INET;
	in.sin_port = PORT;
	if (::bind(this->socket, (sockaddr*) &in, sizeof(in)) == -1)
		return errno;
	return 0;	
}


