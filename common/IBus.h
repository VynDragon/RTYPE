#pragma once

#include <string>

class IBus
{
public:
	virtual int	add(const std::string& address, const std::string& type) = 0;
	virtual	int	in(const std::string& type, const void *data, const std::string& regexString) = 0;
};