#pragma once

#include <Trackable.h>
#include "Sprite.h"
#include "UnitManager.h"

class GraphicsBuffer;

class Coins :public Trackable
{
public:
	Coins(const Sprite& sprite);
	~Coins();

	void addCoins(int amount);
	void draw();

private:
	int mCoins;
	Sprite mSprite;

};