#include "bus.h"
#include <iostream>

Bus::Bus()
{

}

Bus::~Bus()
{
	auto it = modules.begin();
	while (it != modules.end())
	{
		delete it->second;
		it++;
	}
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
	this->iolock.lock();
	this->iolock.unlock();
	return 0;
}

Bus::ModuleWrapper::ModuleWrapper(const getModuleType& getModule, const delModuleType& delModule)
{
	this->module = (*(getModuleType)getModule)();
	this->destructor = (delModuleType)delModule;
}

Bus::ModuleWrapper::~ModuleWrapper()
{
	this->destructor(this->module);
}

int		Bus::ModuleWrapper::setUp(IBus *bus)
{
	return this->module->setUp(bus);
}
int		Bus::ModuleWrapper::input(const std::string& type, const void *data, IBus *bus)
{
	return this->module->input(type, data, bus);
}
int		Bus::ModuleWrapper::tearDown(IBus *bus)
{
	return this->module->tearDown(bus);
}

Bus::BusMessage::BusMessage(const std::string& destination, const std::string & type, Bus::BusMessage::messageData *data)
{
	this->type = type;
	this->data = data;
	this->destination = destination;
}

Bus::BusMessage::~BusMessage()
{
	if (data)
	{
		data->lock.lock();
		data->usrnbr -= 1;
		if (data->usrnbr <= 0 && data->data)
		{
			data->deleteData(data->data);
		}
		data->lock.unlock();
	}
}

inputRunnable	Bus::out()
{
	this->iolock.lock();
	if (this->queue.size() > 0)
	{
		BusMessage* message = this->queue.front();
		this->queue.pop();
		this->iolock.unlock();
		return inputRunnable(true, this->modules[message->destination], message->type, message->data->data, this);
	}
	this->iolock.unlock();
	return inputRunnable(false);
}
