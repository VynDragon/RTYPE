#include "Module_Game.h"
#include "../module_network/msg.h"
#include "../module_config/msg.h"
#include "../module_sfml/msg.h"
#include "../common/templatedTools.h"
#include "../common/messages.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <tuple>

#ifdef CLIENT
const std::map<std::string, tfunctionType> Module_Game::tfunctions = {
	{MSG_CONFIG_RESULT, &Module_Game::config},
	{MSG_SFML_EVENT, &Module_Game::sfmlevent},
	{MSG_EXIT, &Module_Game::exit},
};
#else
const std::map<std::string, tfunctionType> Module_Game::tfunctions = {
	{MSG_NETWORK_JOIN, &Module_Game::playerjoin},
	{MSG_NETWORK_LEAVE, &Module_Game::playerleave},
	{MSG_NETWORK_OUTWITHAUTHOR, &Module_Game::msg},
	{MSG_TICK, &Module_Game::tick}
};
#endif

Module_Game::Module_Game()
{
}

Module_Game::~Module_Game()
{


}
int	Module_Game::setUp(IBus *bus)
{
#ifdef CLIENT
#else
	bus->in(MSG_NETWORK_BIND, new int(3100), delFunction<int*>, "network");
#endif
	return 0;
}
int	Module_Game::input(const std::string& type, const void *data, IBus *bus)
{
	auto doot = tfunctions.find(type);
	if (doot != tfunctions.end())
	{
		return (this->*((*doot).second))(data, bus);
	}
	return 0;
}
int	Module_Game::tearDown(IBus *bus)
{
	(void)bus;
	return 0;
}

#if not defined(CLIENT)
int	Module_Game::playerjoin(const void *data, IBus *bus)
{
	(void)bus;
	std::string	*sdata = (std::string*)data;
	std::cout << "Player joined" << std::endl;
	if (games.size() == 0)
		games.emplace_back();
	if (games.back().addPlayer(*sdata))
	{
		games.emplace_back();
		games.back().addPlayer(*sdata);
	}
	return 0;
}

int	Module_Game::tick(const void *data, IBus *bus)
{
	(void)data;
	for (auto it = games.begin(); it != games.end(); it++)
	{
		it->tick();
		it->sendDraw(bus);
	}
	return 0;
}

int	Module_Game::playerleave(const void *data, IBus *bus)
{
	std::cout << "Player left" << std::endl;
	(void)bus;
	std::string *sdata = (std::string*)data;
	for (auto it = games.begin(); it != games.end(); it++)
	{
		if (it->removePlayer(*sdata, bus) == 0)
			return 0;
	}
	return 1;
}

int	Module_Game::msg(const void *data, IBus *bus)
{
	std::tuple<std::string, std::string,const void*>* sdata = (std::tuple<std::string, std::string ,const void*>*)data;
	//std::cout << std::get<0>(*sdata) << " sent " << std::get<1>(*sdata) << " with " << (void*)std::get<2>(*sdata) << std::endl;
	
	if (std::get<1>(*sdata) == MSG_SFML_EVENT)
	{
		sf::Event *event = (sf::Event*)std::get<2>(*sdata);
		if (event->type == sf::Event::KeyPressed)
		{
			Player *soos = nullptr;
			for (auto it = games.begin(); it != games.end(); it++)
			{
				soos = it->getPlayerById(std::get<0>(*sdata));
				if (soos != nullptr)
					break;
			}
			if (soos == nullptr)
				return 1;
			switch (event->key.code)
			{
				case sf::Keyboard::Up : soos->moveY(-KEY_AMOUNT_MOVE);
					break;
				case sf::Keyboard::Down : soos->moveY(KEY_AMOUNT_MOVE);
					break;
				case sf::Keyboard::Left : soos->moveX(-KEY_AMOUNT_MOVE);
					break;
				case sf::Keyboard::Right : soos->moveX(KEY_AMOUNT_MOVE);
					break;
			}
					
		}
	}
	return 0;
}

#endif

int	Module_Game::sfmlevent(const void *data, IBus *bus)
{
	sf::Event *event = (sf::Event*)data;
	if (event->type == sf::Event::Closed)
	{
		bus->in(MSG_NETWORK_EXPORT_LEAVE, nullptr, nullptr, "network");
		for (int i = 0; i < MAX_THREADS; i++)
		{
			bus->in(MSG_EXIT, nullptr, nullptr);
		}
		return EXIT_RETURN;
	}
	else if (event->type == sf::Event::KeyPressed)
	{
		sf::Event *evcp = new sf::Event;
		evcp->type = sf::Event::KeyPressed;
		evcp->key.code = event->key.code;
		bus->in(MSG_NETWORK_EXPORT, new std::tuple<std::string, std::string, size_t, const void*>("game", MSG_SFML_EVENT, sizeof(sf::Event::EventType) + sizeof(sf::Event::KeyEvent), evcp), [](const void *data) -> void {
				std::tuple<std::string, std::string, size_t, const void*>* sdata = (std::tuple<std::string, std::string, size_t, const void*>*)data;
				delete (uint8_t*)std::get<3>(*sdata);
				delete sdata;
		} , "network");
	}
	return 0;
}
int	Module_Game::exit(const void *data, IBus *bus)
{
	(void)data;
	(void)bus;
	return EXIT_RETURN;
}
int	Module_Game::config(const void *data, IBus *bus)
{
	std::pair<std::string, std::string>* sdata = (std::pair<std::string, std::string>*)data;
	if (sdata->first == "serverip")
	{
		bus->in(MSG_NETWORK_ADDCLIENT, new std::tuple<std::string, std::string, int>("server", sdata->second, PORT), delFunction<std::tuple<std::string, std::string, int>*>, "network");
		bus->in(MSG_NETWORK_EXPORT, new std::tuple<std::string, std::string, size_t, const void*>(".*", "NINININI", 0, nullptr), delFunction<std::tuple<std::string, std::string, size_t, const void*>*>, "network");
	}
	return 0;
}
