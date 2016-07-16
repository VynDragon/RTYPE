#include "Engine.h"

Engine::Worker::Worker(Engine* engine)
{
	this->engine = engine;
}

Engine::Worker::~Worker()
{
	if (this->thread)
		delete this->thread;
}

void Engine::Worker::start()
{
	if (!this->thread)
	{
		this->thread = new std::thread(&Engine::Worker::run, engine);
		this->thread->detach();
	}
	else
		throw EngineException("Worker already has a thread!");
}

void Engine::Worker::stop()
{
	if (this->thread)
	{
		delete this->thread;
		this->thread = 0;
	}
	else
		throw EngineException("Worker has no thread!");
}

void Engine::Worker::run(Engine *engine)
{
	try
	{
		int ret = engine->doWork();
		while (ret != EXIT_RETURN)
		{
			ret = engine->doWork();
		}
	}
	catch(ExitException e)
	{
	}
	engine->threadExited += 1;
	return;
}