#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <map>
#include <vector>

class Module_Game;

#define KEY_AMOUNT_MOVE 0.05f

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
	int		setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	int		sendDraw(IBus *bus, const std::vector<Player>& to) const;
	int		unDraw(IBus *bus, const std::vector<Player>& to) const;
private:
	std::string	networkId;
	float		x = 0;
	float		y = 0;
	uint8_t		r = 255, g = 255, b = 255, a = 255;
};

class Game
{
public:
	Game();
	~Game();
	int			addPlayer(const std::string& networkId);
	int			removePlayer(const std::string& networkId, IBus* bus);
	int			sendDraw(IBus *bus) const;
	Player			*getPlayerById(const std::string& networkId);
	int			tick();
private:
	void			starField(IBus *bus) const;
	float			fieldOffset = 0;
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