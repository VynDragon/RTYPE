#pragma once

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <mutex>
#include <thread>

#include "bus.h"

class EngineException : public std::exception
{
private:
	std::string message;
public:
	EngineException(std::string message) : std::exception(), message(message)
	{
	}
	virtual const char* what() const throw()
	{
		return message.c_str();
	}
};

class Engine
{
public:
	class Worker
	{
	public:
		Worker(Engine* engine);
		~Worker();
		void		start();
		void		stop();
		static void	run(Engine *engine);
	private:
		Worker();
		Engine		*engine;
		std::thread	*thread = 0;
	};
	Engine(const std::vector<std::string>& modules, const std::map<std::string, std::string>& startModules, int nbworker);
	~Engine();
	void		start();
	void		stop();
	int			doWork();
private:
	Engine();
	std::mutex		work;
	std::vector<Worker*>	workers;
public:
	Bus			*bus;
};