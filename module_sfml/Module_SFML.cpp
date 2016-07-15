#include "Module_SFML.h"
#include "../common/messages.h"
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
	(void)bus;
	this->window = new sf::RenderWindow(sf::VideoMode(1024, 768), "RTYPE");
	return 0;
}
int	Module_SFML::input(const std::string& type, const void *data, IBus *bus)
{
	auto doot = tfunctions.find(type);
	if (doot != tfunctions.end())
	{
		return (this->*((*doot).second))(data, bus);
	}
	return 0;
}
int	Module_SFML::tearDown(IBus *bus)
{
	(void)bus;
	if (this->window->isOpen())
	{
		this->window->close();
	}
	delete this->window;
	return 0;
}

int	Module_SFML::tick(const void *data, IBus *bus)
{
	(void)data;
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
        update(data, bus); // good enough if this engine isn't reused
        return 0;
}

int	Module_SFML::addSprite(const void *data, IBus *bus)
{
	(void)bus;
	std::pair<std::string, std::string>* sdata = (std::pair<std::string, std::string>*)data;
	auto it = textures.find(sdata->second);
	if (it == textures.end())
	{
		sf::Texture *texture = new sf::Texture;
		if (!texture->loadFromFile(sdata->second))
			return 1;
		textures[sdata->second] = texture;
	}
	if (sprites.count(sdata->first))
	{
		sprites[sdata->first]->setTexture(*textures[sdata->second], true);
	}
	else
		sprites[sdata->first] = new sf::Sprite(*textures[sdata->second]);
	sf::Vector2u texSize= textures[sdata->second]->getSize();
	sprites[sdata->first]->setOrigin(texSize.x / 2, texSize.y / 2);
	return 0;
}

int	Module_SFML::removeSprite(const void *data, IBus *bus)
{
	(void)bus;
	std::string* sdata = (std::string*)data;
	auto it = sprites.find(*sdata);
	if (it != sprites.end())
	{
		delete it->second;
		sprites.erase(it);
	}
	return 0;
}

int	Module_SFML::setSpritePos(const void *data, IBus *bus)
{
	(void)bus;
	std::tuple<std::string, float, float>* sdata = (std::tuple<std::string, float, float>*)data;
	auto it = sprites.find(std::get<0>(*sdata));
	if (it != sprites.end())
	{
		it->second->setPosition(std::get<1>(*sdata), std::get<1>(*sdata));
		return 0;
	}
	return 1;
}

int	Module_SFML::update(const void *data, IBus *bus)
{
	(void)data;
	(void)bus;
	window->clear();
	for (auto it = sprites.begin(); it != sprites.begin(); it++)
	{
		window->draw(*it->second);
	}
	window->display();
	return 0;
}
