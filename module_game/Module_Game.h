#pragma once

#include "../common/common.h"
#include "../common/module.h"
#include <map>
#include <vector>
#include <cstdint>
#include <random>

class Module_Game;

#define KEY_AMOUNT_MOVE 0.05f

typedef int (Module_Game::*tfunctionType)(const void*, IBus*);

class Drawable
{
public:
	Drawable(const std::string& networkId, const std::string& texture);
	~Drawable();
	int		moveX(float offset);
	int		moveY(float offset);
	float		getX() const;
	float		getY() const;
	int		setXY(float x, float y);
	int		setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	int		sendDraw(IBus *bus, const std::vector<std::string>& to) const;
	int		unDraw(IBus *bus, const std::vector<std::string>& to) const;
	std::string	getNetworkId() const;
protected:
	std::string	networkId;
	float		x = 0;
	float		y = 0;
	uint8_t		r = 255, g = 255, b = 255, a = 255;
	std::string	texture;
};

class Player : public Drawable
{
public:
	Player(const std::string& networkId);
	~Player();
private:
	
};

class Bydos : public Drawable
{
public:
	Bydos(const std::string& networkId, IBus *bus);
	~Bydos();
private:
	std::default_random_engine	rand;
	IBus				*bus;
};

class Game
{
public:
	Game();
	~Game();
	int				addPlayer(const std::string& networkId);
	int				removePlayer(const std::string& networkId, IBus* bus);
	int				sendDraw(IBus *bus) const;
	Player				*getPlayerById(const std::string& networkId);
	Bydos				*getBydosById(const std::string& networkId);
	int				tick(IBus *bus);
private:
	void				starField(IBus *bus) const;
	void				computeColors();
	float				fieldOffset = 0;
	std::vector<Player>		players;
	std::vector<Bydos>		bydoses;
	std::default_random_engine	rand;
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
	int							bydos_move(const void *data, IBus *bus);
	static const std::map<std::string, tfunctionType>	tfunctions;
#ifdef CLIENT
#else
	std::vector<Game>					games;
#endif
};