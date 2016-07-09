#pragma once

#include <string>

typedef void (*destructorType)(const void*);
typedef const destructorType destructorTypeConst;

class IBus
{
public:
	virtual int	add(const std::string& address, const std::string& type) = 0;
	virtual	int	in(const std::string& type, const void *data, destructorTypeConst deleteData, const std::string& regexString = ".*") = 0;
};