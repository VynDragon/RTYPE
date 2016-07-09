#include "Engine.h"
#include <chrono>
#include <ratio>

Engine::Engine(const std::vector<std::string>& modules, const std::map<std::string, std::string>& startModules, int nbworker)
{
  auto module = modules.begin();
  auto startModule = startModules.begin();
  Bus *bbus = new Bus();
  while (module != modules.end())
  {
	bbus->addModule(*module);
	module++;
  }
  while (startModule != startModules.end())
  {
	bbus->add(startModule->first, startModule->second);
	startModule++;
  }
  bus = bbus;
  for (int i = 0; i < nbworker; i++)
  {
	  workers.push_back(new Worker(this));
  }
}

Engine::~Engine()
{
	delete bus;
	auto worker = workers.begin();
	while (worker != workers.end())
	{
		delete *worker;
		worker++;
	}
}

int	Engine::doWork()
{
	work.lock();
	inputRunnable workCharge = bus->out();
	while (!workCharge.valid)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		workCharge = bus->out();
	}
	work.unlock();
	int ret = workCharge.module->input(workCharge.message->type, workCharge.message->data, workCharge.bus);
	delete workCharge.message;
	return ret;
}

void		Engine::start()
{
	for (auto it = workers.begin(); it != workers.end(); it++)
	{
		(*it)->start();
	}
}