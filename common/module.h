#pragma once

#include "common.h"
#include "IBus.h"
#include <string>


class Module
{
public:
	virtual int		setUp(IBus *bus) = 0;
	virtual int		input(const std::string& type, const void *data, IBus *bus) = 0;
	virtual int		tearDown(IBus *bus) = 0;
private:

};

//typedef int (Module::*moduleInputType)(const std::string&, const void*, IBus*);
typedef Module* (*getModuleType)();
typedef void (*delModuleType)(void*);
typedef const char* (*getModuleNameType)();

#define MODULE_DEFINE_FUNCTIONS(name, classname) \
extern "C" DLL_EXPORT Module		*getModule()\
{\
		return new classname();\
}\
extern "C" DLL_EXPORT void			delModule(void *myclass)\
{\
	delete ((classname*)myclass);\
}\
extern "C" DLL_EXPORT const char	*getModuleName()\
{\
	return name;\
}
