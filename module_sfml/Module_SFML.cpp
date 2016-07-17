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
	{MSG_SFML_UPDATE, &Module_SFML::update},
	{MSG_SFML_ADD_SPRITE_NETWORK, &Module_SFML::addSpriteNetwork},
	{MSG_SFML_SETPOSITION_SPRITE_NETWORK, &Module_SFML::setSpritePosNetwork},
	{MSG_SFML_SETCOLOR_SPRITE, &Module_SFML::setSpriteColor},
	{MSG_SFML_SETCOLOR_SPRITE_NETWORK, &Module_SFML::setSpriteColorNetwork},
	{MSG_SFML_REMOVE_SPRITE_NETWORK, &Module_SFML::removeSpriteNetwork}
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
	this->window = new sf::RenderWindow(sf::VideoMode(500, 500), "RTYPE");
	this->view = new sf::View();
	view->reset(sf::FloatRect(0, 0, 1.0f, 1.0f));
	view->setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window->setView(*view);
	window->setActive(false);
	return 0;
}
int	Module_SFML::input(const std::string& type, const void *data, IBus *bus)
{
	window->setActive(true);
	auto doot = tfunctions.find(type);
	int ret = 0;
	if (doot != tfunctions.end())
	{
		ret = (this->*((*doot).second))(data, bus);
	}
	window->setActive(false);
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
	delete this->view;
	return 0;
}

int	Module_SFML::tick(const void *data, IBus *bus)
{
	(void)data;
	sf::Event *event = new sf::Event();
	while (window->pollEvent(*event))
        {
		/*if (event->type == sf::Event::Closed)
		{
			bus->in(MSG_EXIT, nullptr, nullptr, "");
			delete event;
		}
		else
		{*/
			bus->in(MSG_SFML_EVENT, event, delFunction<sf::Event*>, ".*");
		//}
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
	sprites[sdata->first]->setScale (0.01f, 0.01f);
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
		it->second->setPosition(std::get<1>(*sdata), std::get<2>(*sdata));
		return 0;
	}
	return 1;
}

int	Module_SFML::update(const void *data, IBus *bus)
{
	(void)data;
	(void)bus;
	window->clear();
	for (auto it = sprites.begin(); it != sprites.end(); it++)
	{
		window->draw(*it->second);
	}
	window->display();
	return 0;
}

int	Module_SFML::addSpriteNetwork(const void *data, IBus *bus)
{
	char *id = (char*)data;
	char *path = id;
	while (*path != '=' && *path != 0 && path - id < MAX_BUFFER_SIZE) 
		path++;
	if (*path != '=')
		return 1;
	*path = 0;
	path++;
	std::pair<std::string, std::string> pair(id, path);
	addSprite(&pair, bus);
	return 0;
}

int	Module_SFML::setSpritePosNetwork(const void *data, IBus *bus)
{
	char *id = (char*)data;
	float *x = 0;
	float *y = 0;
	int soos = 0;
	while (id[soos] != 0 && (id + soos * sizeof(char)) - id < MAX_BUFFER_SIZE)
		soos++;
	if (id[soos] != 0)
		return 1;
	/*x = (float*)(&id[soos + 1]); HAPPY LITTLE BUGS
	y = x + sizeof(float);*/
	uint8_t* workptr = (uint8_t*)&id[soos];
	workptr++;
	x = (float*)workptr;
	workptr += sizeof(float);
	y = (float*)workptr;
	std::tuple<std::string, float, float> pair(id, *x, *y);
	setSpritePos(&pair, bus);
	return 0;
}

int		Module_SFML::setSpriteColor(const void *data, IBus *bus)
{
	(void)bus;
	std::tuple<std::string, uint8_t, uint8_t,uint8_t, uint8_t>* sdata = (std::tuple<std::string, uint8_t, uint8_t,uint8_t, uint8_t>*)data;
	auto it = sprites.find(std::get<0>(*sdata));
	if (it != sprites.end())
	{
		it->second->setColor(sf::Color(std::get<1>(*sdata), std::get<2>(*sdata), std::get<3>(*sdata), std::get<4>(*sdata)));
		return 0;
	}
	return 1;
}	
int		Module_SFML::setSpriteColorNetwork(const void *data, IBus *bus)
{
	char *id = (char*)data;
	uint8_t *r = 0;
	uint8_t *g = 0;
	uint8_t *b = 0;
	uint8_t *a = 0;
	int soos = 0;
	while (id[soos] != 0 && (id + soos * sizeof(char)) - id < MAX_BUFFER_SIZE)
		soos++;
	if (id[soos] != 0)
		return 1;
	r = (uint8_t*)(&id[soos + 1]);
	g = r + sizeof(uint8_t);
	b = g + sizeof(uint8_t);
	a = b + sizeof(uint8_t);
	std::tuple<std::string, uint8_t, uint8_t, uint8_t, uint8_t> pair(id, *r, *g, *b, *a);
	setSpriteColor(&pair, bus);
	return 0;
}


int		Module_SFML::removeSpriteNetwork(const void *data, IBus *bus)
{
	char *id = (char*)data;
	std::string soos(id);
	removeSprite(&id, bus);
	return 0;
}