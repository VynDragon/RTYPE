#include "Module_Network.h"
#include "../module_config/msg.h"
#include "../common/messages.h"
#include "../common/templatedTools.h"
#include "msg.h"
#include <iostream>
#ifdef OS_LINUX
#include <unistd.h>
#include <poll.h>
#endif
#include <tuple>
#include <cstring>

const std::map<std::string, tfunctionType> Module_Network::tfunctions = {
	{MSG_NETWORK_EXPORT, &Module_Network::send_msg},
	{MSG_NETWORK_EXPORT_ACK, &Module_Network::send_msg_ack},
	{MSG_NETWORK_BIND, &Module_Network::bind},
	{MSG_TICK, &Module_Network::tick},
	{MSG_NETWORK_ADDCLIENT, &Module_Network::addclient},
	{MSG_NETWORK_EXPORT_LEAVE, &Module_Network::exportleave} // can't put a lmabda here :( [](const void *data, IBus *bus) -> int { sendICP(nullptr, 0, 3);return 0; }
};


const std::map<uint8_t, msgFunctionType> Module_Network::msgfunctions = {
	{0, &Module_Network::id0},
	{1, &Module_Network::id1},
	{2, &Module_Network::id2},
	{3, &Module_Network::id3}
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
	(void)bus;
	return 0;
}
int	Module_Network::input(const std::string& type, const void *data, IBus *bus)
{
	auto doot = tfunctions.find(type);
	if (doot != tfunctions.end())
	{
		return (this->*((*doot).second))(data, bus);
	}
	return 0;
}
int	Module_Network::tearDown(IBus *bus)
{
	(void)bus;
	close(this->socket);
	return 0;
}

int	Module_Network::send_msg(const void *data, IBus *bus)
{
	(void)bus;
	std::tuple<std::string, std::string, size_t, const void*> *datee = (std::tuple<std::string, std::string, size_t, const void*> *)data;
	ICPMsg *icpmsg = (ICPMsg *)new uint8_t[std::get<0>(*datee).size() + std::get<1>(*datee).size() + std::get<2>(*datee) + sizeof(ICPMsg) + sizeof(uint16_t) + 2]();
	icpmsg->identifier = 1;
	icpmsg->datasize = std::get<2>(*datee);
	memcpy(icpmsg->data, std::get<1>(*datee).c_str(), std::get<1>(*datee).size() + 1);
	memcpy(icpmsg->data + std::get<1>(*datee).size() + 1, std::get<0>(*datee).c_str(), std::get<0>(*datee).size() + 1);
	memcpy(icpmsg->data + std::get<1>(*datee).size()  + std::get<0>(*datee).size() + 2, &std::get<2>(*datee), sizeof(uint16_t));
	memcpy(icpmsg->data + std::get<1>(*datee).size()  + std::get<0>(*datee).size() + sizeof(uint16_t) + 2, std::get<3>(*datee), std::get<2>(*datee));
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		sendto(this->socket, icpmsg, std::get<0>(*datee).size() + std::get<1>(*datee).size() + std::get<2>(*datee) + sizeof(ICPMsg) + sizeof(uint16_t) + 2, 0, (sockaddr*)&it->second, sizeof(sockaddr_in));
	}
	return 0;	
}

int	Module_Network::send_msg_ack(const void *data, IBus *bus)
{
	return 0;	
}

int	Module_Network::exportleave(const void *data, IBus *bus)
{
	sendICP(nullptr, 0, 3);
	clients.clear();
	return 0;	
}

int	Module_Network::bind(const void *data, IBus *bus)
{
	(void)bus;
	int* port = (int*)data;
	sockaddr_in in = {};
	in.sin_addr.s_addr = htonl(INADDR_ANY);
	in.sin_family = AF_INET;
	in.sin_port = htons(*port);
	if (::bind(this->socket, (sockaddr*) &in, sizeof(in)) == -1)
		return errno;
	return 0;	
}

int	Module_Network::tick(const void *data, IBus *bus)
{
	(void)data;
	uint8_t	buffer[MAX_BUFFER_SIZE] = { 0 };
	struct pollfd	fds[1];
	fds[0].fd = socket;
	fds[0].events = POLLIN;
	int evs = poll(fds, 1, 0);
	if (evs == 0)
		return 0;
	if (evs == -1)
		return evs;
	if (fds[0].revents == POLLIN)
	{
		sockaddr_in	innit = {};
		socklen_t	innitsize = sizeof(sockaddr_in);
		int outish = recvfrom(fds[0].fd, buffer, MAX_BUFFER_SIZE, 0, (sockaddr*)&innit, &innitsize);
		if (outish == -1)
			return outish;
		bool found = false;
		std::string client;
		for (auto it = clients.begin(); it != clients.end(); it++)
		{
			if (it->second.sin_addr.s_addr == innit.sin_addr.s_addr && it->second.sin_port == innit.sin_port)
			{
				found = true;
				client = it->first;
				break;
			}
		}
		if (!found)
		{
			int x = 1;
			std::string namu = "client_";
			namu += std::to_string(x);
			while (clients.find(namu) != clients.end())
			{
				x++;
				namu = "client_";
				namu += std::to_string(x);
			}
			clients[namu] = innit;
			client = namu;
			bus->in(MSG_NETWORK_JOIN, new std::string(namu), delFunction<std::string*>);
		}
		ICPMsg *msg = (ICPMsg*)&buffer;
		auto moom = msgfunctions.find(msg->identifier);
		if (moom != msgfunctions.end())
		{
			return (this->*((*moom).second))(client, msg, bus);
		}
		else
			return 1;
	}
	else return fds[0].revents;
	return 0;
}

int			Module_Network::id0(std::string author, ICPMsg *msg, IBus *bus)  // do nothing
{
	(void)author;
	(void)msg;
	(void)bus;
	return 0;
}
int			Module_Network::id1(std::string author, ICPMsg *msg, IBus *bus) // this is a normal message
{
	uint8_t 	*type = msg->data;
	uint8_t 	*regex = msg->data;
	int		x = 0;
	uint16_t	*size;
	void		*data;
	while (*regex != 0)
	{
		regex++;
	}
	regex++;
	while (regex[x] != 0)
	{
		x++;
	}
	size = (uint16_t*)&regex[x+1];
	data = size; // for some reason data = size + sizeof(uint16_t) was giving size + 4 instead of size + 2 ??? 1 bit shifting going on?
	data += sizeof(uint16_t);
	void *datacopy = new uint8_t[*size];
	memcpy(datacopy, data, *size);
	bus->in(std::string((char*)type), datacopy, delFunction<uint8_t*>);
	datacopy = new uint8_t[*size];
	memcpy(datacopy, data, *size);
	bus->in(MSG_NETWORK_OUTWITHAUTHOR, new std::tuple<std::string, std::string, const void*>(author, std::string((char*)type), datacopy), [](const void* dateee) -> void { std::tuple<std::string, std::string, const void*> *dat = (std::tuple<std::string, std::string, const void*>*)dateee; delete  (uint8_t*)std::get<2>(*dat); delete dat;});
	return 0;
}
int			Module_Network::id2(std::string author, ICPMsg *msg, IBus *bus) // this is an important message
{
	
	uint8_t 	*type = msg->data;
	uint8_t 	*regex = msg->data;
	int		x = 0;
	uint16_t	*size;
	void		*data;
	while (*regex != 0)
	{
		regex++;
	}
	regex++;
	while (regex[x] != 0)
	{
		x++;
	}
	size = (uint16_t*)&regex[x+1];
	data = size;
	data += sizeof(uint16_t);
	ICPMsg icpmsg;
	icpmsg.identifier = 0;
	auto addr = clients.find(author);
	sendto(this->socket, &icpmsg, sizeof(ICPMsg), 0, (sockaddr*)&addr->second, sizeof(sockaddr_in));
	void *datacopy = new uint8_t[*size];
	memcpy(datacopy, data, *size);
	bus->in(std::string((char*)type), datacopy, delFunction<uint8_t*>);
	datacopy = new uint8_t[*size];
	memcpy(datacopy, data, *size);
	bus->in(MSG_NETWORK_OUTWITHAUTHOR, new std::tuple<std::string, std::string, const void*>(author, std::string((char*)type), datacopy), [](const void* dateee) -> void { std::tuple<std::string, std::string, const void*> *dat = (std::tuple<std::string, std::string, const void*>*)dateee; delete  (uint8_t*)std::get<2>(*dat); delete dat;});
	return 0;
}
int			Module_Network::id3(std::string author, ICPMsg *msg, IBus *bus) // this is the client leaving
{
	(void)msg;
	auto addr = clients.find(author);
	if (addr == clients.end())
		return 0;// uuuhh what? shouldn't happen
	bus->in(MSG_NETWORK_LEAVE, new std::string(addr->first), delFunction<std::string*>);
	clients.erase(addr);
	return 0;
}

int			Module_Network::addclient(const void *data, IBus *bus)
{
	std::tuple<std::string, std::string, int>* sdata = (std::tuple<std::string, std::string, int>*)data;
	struct sockaddr_in sin;
	inet_pton(AF_INET, std::get<1>(*sdata).c_str(), &(sin.sin_addr));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(std::get<2>(*sdata));
	clients[std::get<0>(*sdata)] = sin;
	return 0;
}

int			Module_Network::sendICP(const void* data, uint16_t size, uint8_t identifier)
{
	ICPMsg *icpmsg = (ICPMsg *)new uint8_t[sizeof(ICPMsg) + size]();
	icpmsg->identifier = identifier;
	icpmsg->datasize = size;
	memcpy(icpmsg->data, data, size);
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		sendto(this->socket, icpmsg, sizeof(ICPMsg) + size, 0, (sockaddr*)&it->second, sizeof(sockaddr_in));
	}
	return 0;
}
