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
#include <mutex>
#include <queue>
#define MODULE_LOAD_ERROR "Module loading error: "
#define MODULE_ADD_ERROR "Module adding error: "

struct inputRunnable;

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
	class BusMessage {
	public:
 		typedef struct messageData {
			messageData(unsigned long usrnbr, const void* data, destructorTypeConst deleteData) : usrnbr(usrnbr), data(data), deleteData(deleteData) {}
			std::mutex		lock;
			unsigned long 		usrnbr;
			const void 		*data;
			destructorTypeConst	deleteData;
		}	messageData;
		messageData	*data;
		std::string	type;
		std::string	destination;
		BusMessage(const std::string& destination, const std::string& type, messageData *data);
		~BusMessage();
	};
	
	typedef struct {
		getModuleType		getModule;
		delModuleType		delModule;
		getModuleNameType	getModuleName;
	}	moduleType;
	
	class ModuleWrapper : public Module
	{
	private:
		Module			*module;
		delModuleType		destructor;
		ModuleWrapper();
	public:
		ModuleWrapper(const getModuleType& getModule, const delModuleType& delModule);
		virtual ~ModuleWrapper();
		int		setUp(IBus *bus);
		int		input(const std::string& type, const void *data, IBus *bus);
		int		tearDown(IBus *bus);
	};
	
	Bus();
	virtual ~Bus();
	int 				addModule(const std::string& module);
	int				add(const std::string& address, const std::string& type);
	int 				in(const std::string& type, const void *data, destructorTypeConst deleteData, const std::string& regexString = ".*");
	inputRunnable			out();
protected:
	std::queue<BusMessage*>			queue; // !! share BusMessage instances's data		
	std::mutex				iolock;
	std::map<std::string, moduleType>	moduleTypes;
	std::map<std::string, ModuleWrapper*>	modules;
};

typedef struct inputRunnable {
	inputRunnable(bool valid, Module *module, Bus::BusMessage *message, IBus *bus)
	{
		this->valid = valid;
		this->module = module;
		this->message = message;
		this->bus = bus;
	}
	inputRunnable(bool valid)
	{
		this->valid = valid;
	}
	bool				valid;
	Module				*module;
	Bus::BusMessage			*message;
	IBus				*bus;
}	inputRunnable;