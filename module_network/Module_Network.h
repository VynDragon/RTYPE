#pragma once

#include "../common/common.h"
#include "../common/module.h"

#ifdef OS_LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined(OS_WINDOWS)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Ws2tcpip.h>
#endif

#include <map>
#include <cstdint>


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
	int							exportleave(const void *data, IBus *bus);
	int							tick(const void *data, IBus *bus);
	int							send_msg_ack(const void *data, IBus *bus);
	int							bind(const void *data, IBus *bus);
	int							addclient(const void *data, IBus *bus);
	

	int							id0(std::string author, ICPMsg *msg, IBus *bus);
	int							id1(std::string author, ICPMsg *msg, IBus *bus);
	int							id2(std::string author, ICPMsg *msg, IBus *bus);
	int							id3(std::string author, ICPMsg *msg, IBus *bus);
	
	int							sendICP(const void* data, uint16_t size, uint8_t identifier);

	static const std::map<std::string, tfunctionType>	tfunctions;
	static const std::map<uint8_t, msgFunctionType>		msgfunctions;
#ifdef OS_LINUX
	int 							socket;
#elif defined(OS_WINDOWS)
	SOCKET 							socket;
	WSADATA							wsaData;
#endif
	bool							waitingEvent;
	std::map<std::string, sockaddr_in>			clients;
};