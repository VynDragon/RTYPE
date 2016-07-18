#pragma once

#include <string>

typedef void (*destructorType)(const void*);
typedef const destructorType destructorTypeConst;

class IBus
{
public:
	virtual int	addModule(const std::string& module) = 0;
	virtual bool	loaded(const std::string& module) = 0;
	virtual bool	exist(const std::string& address) = 0;
	virtual int	add(const std::string& address, const std::string& type) = 0;
	virtual int	remove(const std::string& address) = 0;
	virtual	int	in(const std::string& type, const void *data, destructorTypeConst deleteData, const std::string& regexString = ".*") = 0;
};