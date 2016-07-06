#pragma once

#include "common.h"
#include <string>

class Module
{
public:
	virtual int	in(const std::string& type, const void *data) = 0;
private:

};

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
