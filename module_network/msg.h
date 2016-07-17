#pragma once

// base output is the carbon copy of the messages sent.
#define MSG_NETWORK_OUTWITHAUTHOR "innetworkmsg" // std::tuple<std::string, std::string type,uint8_t[]> id then type then raw data
#define MSG_NETWORK_JOIN "someoneconnected" // std::string id
#define MSG_NETWORK_LEAVE "someonedisconnected" // std::string id
//input
// DATA SENT VIA NETWORK MUSTN'T CONTAIN POINTERS, OTHERWISE IT WILL GET LOST
#define MSG_NETWORK_EXPORT "networkmsg"// data is std::tuple<std::string, std::string, size_t, const void*> as address regex, type, size, data
#define MSG_NETWORK_EXPORT_TARGET "networkmsgtarget"// data is std::tuple<std::string, std::string, std::string, size_t, const void*> as id, address regex, type, size, data
#define MSG_NETWORK_EXPORT_ACK "importantnetworkmsg"// data is std::tuple<std::string, std::string, size_t, const void*> as address regex, type, size, data
#define MSG_NETWORK_EXPORT_LEAVE "weleavin'now"// no data, it sends type 3 packets to all the clients
#define MSG_NETWORK_BIND "timetobind" // data is int as port
#define MSG_NETWORK_ADDCLIENT "networkaddclient" // data is std::tuple<std::string, std::string, int> as id, address, and port 