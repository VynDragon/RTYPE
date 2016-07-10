#include "Engine.h"
#include <chrono>
#include <ratio>

Engine::Engine(const std::vector<std::string>& modules, int nbworker)
{
  auto module = modules.begin();
  Bus *bbus = new Bus();
  while (module != modules.end())
  {
	bbus->addModule(*module);
	module++;
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
		std::this_thread::sleep_for(WAIT_SLEEP_TIME);
		workCharge = bus->out();
	}
	work.unlock();
	int ret = workCharge.module->input(workCharge.message->type, workCharge.message->data->data, workCharge.bus);
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
Bus	*Engine::getBus()
{
	return this->bus;
}