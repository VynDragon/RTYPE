#include "bus.h"
#include <iostream>
#include <dlfcn.h>

Bus::Bus()
{

}

Bus::~Bus()
{

}

int	Bus::addModule(const std::string& module)
{
#ifdef OS_WINDOWS
	HMODULE handle = LoadLibrary(module.c_str());
	if (!handle)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + std::to_string(GetLastError()));
		return GetLastError();
	}
	void *getModule = GetProcAddress(handle, "getModule");
	if (!getModule)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + std::to_string(GetLastError()));
		return GetLastError();
	}
	void *delModule = GetProcAddress(handle, "delModule");
	if (!delModule)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + std::to_string(GetLastError()));
		return GetLastError();
	}
	void *getModuleName = GetProcAddress(handle, "getModuleName");
	if (!getModuleName)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + std::to_string(GetLastError()));
		return GetLastError();
	}
#elif defined(OS_LINUX)
	void *handle = dlopen(module.c_str(), RTLD_LAZY);
	if (!handle)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + dlerror());
		return 1;
	}
	void *getModule = dlsym(handle, "getModule");
	if (!getModule)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + dlerror());
		return 1;
	}
	void *delModule = dlsym(handle, "delModule");
	if (!delModule)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error +dlerror());
		return 1;
	}
	void *getModuleName = dlsym(handle, "getModuleName");
	if (!getModuleName)
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + dlerror());
		return 1;
	}
#endif
	moduleType type = {(getModuleType)getModule, (delModuleType)delModule, (getModuleNameType)getModuleName};
	auto ifType = moduleTypes.find(type.getModuleName());
	if (ifType != moduleTypes.end())
	{
		std::string error = MODULE_LOAD_ERROR;
		throw BusException(error + "didn't find module");
		return 1;
	}
	moduleTypes[type.getModuleName()] = type;
	return 0;
}

int Bus::add(const std::string& address, const std::string& type)
{
	auto itAdr = modules.find(address);
	auto itType = moduleTypes.find(type);
	if (itAdr != modules.end() || itType == moduleTypes.end())
	{
		std::string error = MODULE_ADD_ERROR;
		throw BusException(error + "address is taken, or/and type not found");
		return 1;
	}
	modules[address] = new ModuleWrapper(moduleTypes[type].getModule, moduleTypes[type].delModule);
	return 0;
}

int Bus::in(const std::string& type, const void *data, const std::string& regexString)
{
	return 0;
}

Bus::ModuleWrapper::ModuleWrapper(getModuleType getModule, delModuleType delModule)
{
	this->module = (*(getModuleType)getModule)();
	this->destructor = (delModuleType)getModule;
}

Bus::ModuleWrapper::~ModuleWrapper()
{
}

int Bus::ModuleWrapper::in(const BusMessage & message)
{
	return 0;
}

Bus::BusMessage::BusMessage(const unsigned long & usrnbr, const std::string & type, const void * data, destructorTypeConst deletedata) : data(data), deletedata(deletedata)
{
	this->usrnbr = usrnbr;
	this->type = type;
}

Bus::BusMessage::~BusMessage()
{
}
