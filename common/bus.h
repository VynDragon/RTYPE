#pragma once
#include "../common/common.h"
#include "../common/module.h"
#include "../common/IBus.h"

#ifdef OS_WINDOWS
#include <windows.h>
#elif defined(OS_LINUX)
#include <dlfcn.h>
#endif
#include <exception>
#include <string>
#include <map>
#include <atomic>

#define MODULE_LOAD_ERROR "Module loading error: "
#define MODULE_ADD_ERROR "Module adding error: "

typedef int(*destructorType)();
typedef const destructorType destructorTypeConst;

typedef struct {
	getModuleType		getModule;
	delModuleType		delModule;
	getModuleNameType	getModuleName;
} moduleType;

class BusException : public std::exception
{
private:
	std::string message;
public:
	BusException(std::string message) : std::exception(), message(message)
	{
	}
	virtual const char* what() const throw()
	{
		return message.c_str();
	}
};

class Bus : public IBus
{
public:
	class BusMessage
	{
	private:
		std::atomic_ulong usrnbr;
		const void *data;
		destructorTypeConst deletedata = deletedata;
		std::string type;
	public:
		BusMessage(const unsigned long& usrnbr, const std::string& type, const void *data, destructorTypeConst deletedata);
		~BusMessage();
	};
	class ModuleWrapper
	{
	private:
		Module			*module;
		delModuleType	destructor;
		ModuleWrapper();
	public:
		ModuleWrapper(void *getModule, void *delModule);
		~ModuleWrapper();
		int	in(const BusMessage& message);
	};
	Bus();
	virtual ~Bus();
	int addModule(const std::string& module);
	int	add(const std::string& address, const std::string& type);
	int in(const std::string& type, const void *data, const std::string& regexString = ".*");
protected:
	std::map<std::string, moduleType> moduleTypes;
	std::map<std::string, ModuleWrapper*> modules;
};