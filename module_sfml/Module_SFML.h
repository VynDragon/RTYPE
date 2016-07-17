#pragma once
#include <SFML/Graphics.hpp>

#include "../common/common.h"
#include "../common/module.h"

class Module_SFML;

typedef int (Module_SFML::*tfunctionType)(const void*, IBus*);

class Module_SFML : public Module
{
public:
	Module_SFML();
	virtual ~Module_SFML();
	virtual int						setUp(IBus *bus);
	virtual int						input(const std::string& type, const void *data, IBus *bus);
	virtual int						tearDown(IBus *bus);
private:
	sf::RenderWindow					*window;
	sf::View						*view;
	static const std::map<std::string, tfunctionType>	tfunctions;
	std::map<std::string, sf::Texture*>			textures;
	std::map<std::string, sf::Sprite*>			sprites;
	int							tick(const void *data, IBus *bus);
	int							addSprite(const void *data, IBus *bus);
	int							addSpriteNetwork(const void *data, IBus *bus);
	int							setSpritePosNetwork(const void *data, IBus *bus);
	int							removeSprite(const void *data, IBus *bus);
	int							removeSpriteNetwork(const void *data, IBus *bus);
	int							setSpritePos(const void *data, IBus *bus);
	int							setSpriteColor(const void *data, IBus *bus);
	int							setSpriteColorNetwork(const void *data, IBus *bus);
	int							update(const void *data, IBus *bus);
};