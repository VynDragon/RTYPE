#include "Module_SFML.h"
#include "../module_tick/msg.h"
#include "../common/messages.h"
#include "../common/templatedTools.h"
#include "msg.h"
#include <iostream>

const std::map<std::string, tfunctionType> Module_SFML::tfunctions = {
	{MSG_TICK, &Module_SFML::tick},
	{MSG_SFML_ADD_SPRITE, &Module_SFML::addSprite},
	{MSG_SFML_REMOVE_SPRITE, &Module_SFML::removeSprite},
	{MSG_SFML_SETPOSITION_SPRITE, &Module_SFML::setSpritePos},
	{MSG_SFML_UPDATE, &Module_SFML::update}
};

Module_SFML::Module_SFML()
{
}

Module_SFML::~Module_SFML()
{
}
int	Module_SFML::setUp(IBus *bus)
{
	this->window = new sf::RenderWindow(sf::VideoMode(1024, 768), "RTYPE");
	return 0;
}
int	Module_SFML::input(const std::string& type, const void *data, IBus *bus)
{
	auto doot = tfunctions.find(type);
	if (doot != tfunctions.end())
	{
		(this->*((*doot).second))(data, bus);
	}
	return 0;
}
int	Module_SFML::tearDown(IBus *bus)
{
	if (this->window->isOpen())
	{
		this->window->close();
	}
	delete this->window;
	return 0;
}

void	Module_SFML::tick(const void *data, IBus *bus)
{
	sf::Event *event = new sf::Event();
	while (window->pollEvent(*event))
        {
		if (event->type == sf::Event::Closed)
		{
			bus->in(MSG_EXIT, nullptr, nullptr, "");
			delete event;
		}
		else
		{
			bus->in(MSG_SFML_EVENT, event, delFunction<sf::Event*>, ".*");
		}
		event = new sf::Event();
        }
}

void	Module_SFML::addSprite(const void *data, IBus *bus)
{
	
}

void	Module_SFML::removeSprite(const void *data, IBus *bus)
{
	
}

void	Module_SFML::setSpritePos(const void *data, IBus *bus)
{
	
}

void	Module_SFML::update(const void *data, IBus *bus)
{
	
}
