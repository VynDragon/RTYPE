#include "Module_Game.h"
#include "../module_sfml/msg.h"
#include "../module_network/msg.h"
#include <cstring>
#include <iostream>

Game::Game()
{
	
}

Game::~Game()
{
	
}

int	Game::addPlayer(const std::string& networkId)
{
	if (players.size() >= 4)
		return 1;
	players.emplace_back(networkId);
	switch (players.size())
	{
		case 1:	players.back().setColor(100,100,255,255);
			break;
		case 2:	players.back().setColor(255,100,100,255);
			break;
		case 3:	players.back().setColor(255,255,100,255);
			break;
		case 4:	players.back().setColor(100,255,100,255);
			break;
	}
	players.back().setXY((float)players.size() / 4.0f, players.back().getY());
	return 0;
}

int	Game::removePlayer(const std::string& networkId)
{
	for (auto it = players.begin(); it != players.end(); it++)
	{
		if (it->getNetworkId() == networkId)
			players.erase(it);
		return 0;
	}
	return 1;
}

int	Game::sendDraw(IBus *bus) const
{
	starField(bus);
	for (auto it = players.begin(); it != players.end(); it++)
	{
		int ret = it->sendDraw(bus, players);
		if (ret != 0)
			return ret;
	}
	return 0;
}

int	Game::tick()
{
	fieldOffset += 0.01f;
	if (fieldOffset >= 1.0f)
		fieldOffset = 0.0f;
	return 0;
}

void	Game::starField(IBus *bus) const
{
	for (auto it = players.begin(); it != players.end(); it++)
	{
		size_t size = 17;
		uint8_t *doush = new uint8_t[size];
		memcpy(doush, "field", 5);
		memcpy(doush + 5, "=Field.png", 12);
		bus->in(MSG_NETWORK_EXPORT_TARGET,
			new std::tuple<std::string, std::string, std::string, size_t, const void*>(it->getNetworkId(), ".*", MSG_SFML_ADD_SPRITE_NETWORK, sizeof(uint8_t) * size, doush),
			[](const void *data) -> void {
				std::tuple<std::string, std::string, std::string, size_t, const void*>* sdata = (std::tuple<std::string, std::string, std::string, size_t, const void*>*)data;
				delete (uint8_t*)std::get<4>(*sdata);
				delete sdata;
			});
		size = 6 + 2 * sizeof(float);
		uint8_t *dish = new uint8_t[size];
		memcpy(dish, "field", 6);
		memcpy(dish + 6, &fieldOffset, sizeof(float));
		float sauce = 0;
		memcpy(dish + 6 + sizeof(float), &sauce, sizeof(float));
		
		bus->in(MSG_NETWORK_EXPORT_TARGET,
			new std::tuple<std::string, std::string, std::string, size_t, const void*>(it->getNetworkId(), ".*", MSG_SFML_SETPOSITION_SPRITE_NETWORK, sizeof(uint8_t) * size, dish),
			[](const void *data) -> void {
				std::tuple<std::string, std::string, std::string, size_t, const void*>* sdata = (std::tuple<std::string, std::string, std::string, size_t, const void*>*)data;
				delete (uint8_t*)std::get<4>(*sdata);
				delete sdata;
			});
	}
}
