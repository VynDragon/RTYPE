#pragma once
#include <SFML/Graphics.hpp>

#include "../common/common.h"
#include "../common/module.h"

class Module_SFML;

typedef void (Module_SFML::*tfunctionType)(const void*, IBus*);

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
	static const std::map<std::string, tfunctionType>	tfunctions;
	void							tick(const void *data, IBus *bus);
	void							addSprite(const void *data, IBus *bus);
	void							removeSprite(const void *data, IBus *bus);
	void							setSpritePos(const void *data, IBus *bus);
	void							update(const void *data, IBus *bus);
};