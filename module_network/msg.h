#pragma once

// base output is the carbon copy of the messages sent.
#define MSG_NETWORK_OUTWITHAUTHOR "innetworkmsg" // std::pair<std::string, const void*> id then data
#define MSG_NETWORK_JOIN "someoneconnected" // std::string id
#define MSG_NETWORK_LEAVE "someonedisconnected" // std::string id
//input
#define MSG_NETWORK_EXPORT "networkmsg"// data is std::tuple<size_t, std::string, std::string, const void*> as size, address regex, type ,data
#define MSG_NETWORK_EXPORT_ACK "importantnetworkmsg"// data is std::tuple<size_t, std::string, std::string, const void*> as size, address regex, type ,data
#define MSG_NETWORK_BIND "timetobind" // data is int as port