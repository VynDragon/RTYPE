#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <map>
#include <vector>

class Module_Game;

typedef int (Module_Game::*tfunctionType)(const void*, IBus*);

class Player
{
public:
	Player(const std::string& networkId);
	~Player();
	std::string	getNetworkId() const;
	int		moveX(float offset);
	int		moveY(float offset);
	float		getX() const;
	float		getY() const;
	int		setXY(float x, float y);
	int		sendDraw(IBus *bus, const std::vector<Player>& to) const;
private:
	std::string	networkId;
	float		x = 0.5;
	float		y = 0.5;
};

class Game
{
public:
	Game();
	~Game();
	int			addPlayer(const std::string& networkId);
	int			sendDraw(IBus *bus) const;
private:
	std::vector<Player>	players;
};

class Module_Game : public Module
{
public:
	Module_Game();
	virtual ~Module_Game();
	virtual int						setUp(IBus *bus);
	virtual int						input(const std::string& type, const void *data, IBus *bus);
	virtual int						tearDown(IBus *bus);
private:
	int							playerjoin(const void *data, IBus *bus);
	int							playerleave(const void *data, IBus *bus);
	int							msg(const void *data, IBus *bus);
	int							sfmlevent(const void *data, IBus *bus);
	int							exit(const void *data, IBus *bus);
	int							config(const void *data, IBus *bus);
	int							tick(const void *data, IBus *bus);
	static const std::map<std::string, tfunctionType>	tfunctions;
#ifdef CLIENT
#else
	std::vector<Game>					games;
#endif
};