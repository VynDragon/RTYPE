#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <map>

class Module_Game;

typedef int (Module_Game::*tfunctionType)(const void*, IBus*);

class Module_Game : public Module
{
public:
	Module_Game();
	virtual ~Module_Game();
	virtual int						setUp(IBus *bus);
	virtual int						input(const std::string& type, const void *data, IBus *bus);
	virtual int						tearDown(IBus *bus);
private:
	int							playerjoin(const void *data, IBus *bus);
	int							playerleave(const void *data, IBus *bus);
	int							msg(const void *data, IBus *bus);
	int							sfmlevent(const void *data, IBus *bus);
	int							exit(const void *data, IBus *bus);
	int							config(const void *data, IBus *bus);
	static const std::map<std::string, tfunctionType>	tfunctions;
};