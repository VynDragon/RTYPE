#include "Module_Game.h"
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
	return 0;
}

int	Game::sendDraw(IBus *bus) const
{
	for (auto it = players.begin(); it != players.end(); it++)
	{
		int ret = it->sendDraw(bus, players);
		if (ret != 0)
			return ret;
	}
	return 0;
}