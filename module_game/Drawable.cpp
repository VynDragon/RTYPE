#include "Module_Game.h"
#include "../module_sfml/msg.h"
#include "../module_network/msg.h"
#include "../common/templatedTools.h"
#include <cstring>

Drawable::Drawable(const std::string& networkId, const std::string& texture)
{
	this->networkId = networkId;
	this->texture = texture;
}

Drawable::~Drawable()
{
	
}

int	Drawable::moveX(float offset)
{
	this->x += offset;
	return 0;
}

int	Drawable::moveY(float offset)
{
	this->y += offset;
	return 0;
}

float	Drawable::getX() const
{
	return x;
}
float	Drawable::getY() const
{
	return y;
}

int	Drawable::setXY(float x, float y)
{
	this->x = x;
	this->y = y;
	return 0;
}

int	Drawable::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	return 0;
}

int	Drawable::sendDraw(IBus* bus, const std::vector< std::string >& to) const
{
	for (auto it = to.begin(); it != to.end(); it++)
	{
		size_t size = this->getNetworkId().size() + this->texture.size() + 2;
		std::string composite = "=";
		composite += this->texture;
		uint8_t *doush = new uint8_t[size];
		memcpy(doush, this->getNetworkId().c_str(), this->getNetworkId().size());
		memcpy(doush + this->getNetworkId().size(), composite.c_str(), composite.size() + 1);
		bus->in(MSG_NETWORK_EXPORT_TARGET,
			new std::tuple<std::string, std::string, std::string, size_t, const void*>(*it, ".*", MSG_SFML_ADD_SPRITE_NETWORK, sizeof(uint8_t) * size, doush),
			[](const void *data) -> void {
				std::tuple<std::string, std::string, std::string, size_t, const void*>* sdata = (std::tuple<std::string, std::string, std::string, size_t, const void*>*)data;
				delete (uint8_t*)std::get<4>(*sdata);
				delete sdata;
			});
		size = this->getNetworkId().size() + 1 + sizeof(float) * 2;
		uint8_t *dish = new uint8_t[size];
		memcpy(dish, this->getNetworkId().c_str(), this->getNetworkId().size() + 1);
		memcpy(dish + this->getNetworkId().size() + 1, &x, sizeof(float));
		memcpy(dish + this->getNetworkId().size() + 1 + sizeof(float), &y, sizeof(float));
		bus->in(MSG_NETWORK_EXPORT_TARGET,
			new std::tuple<std::string, std::string, std::string, size_t, const void*>(*it, ".*", MSG_SFML_SETPOSITION_SPRITE_NETWORK, sizeof(uint8_t) * size, dish),
			[](const void *data) -> void {
				std::tuple<std::string, std::string, std::string, size_t, const void*>* sdata = (std::tuple<std::string, std::string, std::string, size_t, const void*>*)data;
				delete (uint8_t*)std::get<4>(*sdata);
				delete sdata;
			});
		size = this->getNetworkId().size() + 1 + sizeof(uint8_t) * 4;
		uint8_t *desh = new uint8_t[size];
		memcpy(desh, this->getNetworkId().c_str(), this->getNetworkId().size() + 1);
		memcpy(desh + this->getNetworkId().size() + 1, &r, sizeof(uint8_t));
		memcpy(desh + this->getNetworkId().size() + 1 + sizeof(uint8_t), &g, sizeof(uint8_t));
		memcpy(desh + this->getNetworkId().size() + 1 + sizeof(uint8_t) * 2, &b, sizeof(uint8_t));
		memcpy(desh + this->getNetworkId().size() + 1 + sizeof(uint8_t) * 3, &a, sizeof(uint8_t));
		bus->in(MSG_NETWORK_EXPORT_TARGET,
			new std::tuple<std::string, std::string, std::string, size_t, const void*>(*it, ".*", MSG_SFML_SETCOLOR_SPRITE_NETWORK, sizeof(uint8_t) * size, desh),
			[](const void *data) -> void {
				std::tuple<std::string, std::string, std::string, size_t, const void*>* sdata = (std::tuple<std::string, std::string, std::string, size_t, const void*>*)data;
				delete (uint8_t*)std::get<4>(*sdata);
				delete sdata;
			});
	}
	return 0;
}

int		Drawable::unDraw(IBus *bus, const std::vector< std::string >& to) const
{
	for (auto it = to.begin(); it != to.end(); it++)
	{
		size_t size = this->getNetworkId().size() + 1;
		uint8_t *doush = new uint8_t[size];
		memcpy(doush, this->getNetworkId().c_str(), this->getNetworkId().size() + 1);
		bus->in(MSG_NETWORK_EXPORT_TARGET, new std::tuple<std::string, std::string, std::string, size_t, const void*>(*it, ".*", MSG_SFML_REMOVE_SPRITE_NETWORK, sizeof(uint8_t) * size, doush), delFunction<char*>);
	}
	return 0;
}

std::string	Drawable::getNetworkId() const
{
	return this->networkId;
}