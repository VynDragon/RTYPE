#pragma once

#include "../common/common.h"
#include "../common/module.h"

#ifdef OS_LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <map>
#include <cstdint>

#define PORT 3100
#define MAX_BUFFER_SIZE 2048

class Module_Network;

struct ICPMsg {
	uint8_t		identifier;
	uint16_t	datasize;
	uint8_t		data[]; // CAST
};


typedef int (Module_Network::*tfunctionType)(const void*, IBus*);
typedef int (Module_Network::*msgFunctionType)(std::string, ICPMsg*, IBus*);

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
	int							tick(const void *data, IBus *bus);
	int							send_msg_ack(const void *data, IBus *bus);
	int							bind(const void *data, IBus *bus);
	
	int							id0(std::string author, ICPMsg *msg, IBus *bus);
	int							id1(std::string author, ICPMsg *msg, IBus *bus);
	int							id2(std::string author, ICPMsg *msg, IBus *bus);
	int							id3(std::string author, ICPMsg *msg, IBus *bus);
	
	static const std::map<std::string, tfunctionType>	tfunctions;
	static const std::map<uint8_t, msgFunctionType>		msgfunctions;
	int 							socket;
	bool							waitingEvent;
	std::map<std::string, sockaddr_in>			clients;
};