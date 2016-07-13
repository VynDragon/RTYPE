#pragma once

#include "../common/common.h"
#include "../common/module.h"

#ifdef OS_LINUX
#include <sys/socket.h>
#endif

#include <map>

#define PORT 3100

class Module_Network;

typedef int (Module_Network::*tfunctionType)(const void*, IBus*);

class Module_Network : public Module
{
public:
	Module_Network();
	virtual ~Module_Network();
	virtual int						setUp(IBus *bus);
	virtual int						input(const std::string& type, const void *data, IBus *bus);
	virtual int						tearDown(IBus *bus);
	private:
	int							send_msg(const void *data, IBus *bus);
	int							send_msg_ack(const void *data, IBus *bus);
	int							bind(const void *data, IBus *bus);
	static const std::map<std::string, tfunctionType>	tfunctions;
	int 							socket;
};