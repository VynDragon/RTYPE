#include "bus.h"
#include <iostream>
#include <regex>

Bus::Bus()
{

}

Bus::~Bus()
{
	auto it = modules.begin();
	while (it != modules.end())
	{
		it->second->tearDown(this);
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
		throw BusException(error + "module already exist");
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
	if (modules[address]->setUp(this) != 0)
	{
		std::string error = MODULE_ADD_ERROR;
		error += "setup of module ";
		error += address;
		throw BusException(error + " returned non zero value");
		return 2;
	}
	return 0;
}

int Bus::in(const std::string& type, const void *data, destructorTypeConst deleteData, const std::string& regexString)
{
	this->iolock.lock();
	std::regex regex(regexString);
	BusMessage::messageData *mdata = new BusMessage::messageData(1, data, deleteData);
	mdata->lock.lock();
	for (auto it = modules.begin(); it != modules.end(); it++)
	{
		if (std::regex_match(it->first, regex, std::regex_constants::match_not_null | std::regex_constants::match_default))
		{
			mdata->usrnbr += 1;
			queue.push(new BusMessage(it->first, type, mdata));
		}
	}
	if (mdata->usrnbr <= 1)
	{
		mdata->lock.unlock();
		delete mdata;
	}
	mdata->usrnbr -= 1;
	mdata->lock.unlock();
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
	inputLock.lock();
	auto ret = this->module->input(type, data, bus);
	inputLock.unlock();
	return ret;
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
			if (data->deleteData)
				data->deleteData(data->data);
			data->lock.unlock();
			delete data;
		}
		else if (data->usrnbr <= 0)
		{
			data->lock.unlock();
			delete data;
		}
		else
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
		auto mod = this->modules.find(message->destination);
		if (mod == this->modules.end())
		{
			delete message;
			return inputRunnable(false);
		}
		return inputRunnable(true, mod->second, message, this);
	}
	this->iolock.unlock();
	return inputRunnable(false);
}

int		Bus::remove(const std::string& address)
{
	this->iolock.lock();
	auto itAdr = modules.find(address);
	if (itAdr == modules.end())
	{
		std::string error = MODULE_REMOVE_ERROR;
		throw BusException(error + "module instance not found");
		return 1;
	}
	if (modules[address]->tearDown(this) != 0)
	{
		std::string error = MODULE_REMOVE_ERROR;
		error += "tearDown of module ";
		error += address;
		throw BusException(error + " returned non zero value");
		return 1;
	}
	delete modules[address];
	modules.erase(address);
	this->iolock.unlock();
	return 0;
}


bool		Bus::loaded(const std::string& module)
{
	auto that = this->moduleTypes.find(module);
	return !(that == this->moduleTypes.end()); 
}
bool		Bus::exist(const std::string& address)
{
	auto that = this->modules.find(address);
	return !(that == this->modules.end()); 
}