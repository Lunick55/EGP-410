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
	void update();

	void setID(int myID) { mID = myID; };
	int getID() { return mID; };

private:
	int mCoins;
	Sprite mSprite;
	int mID;
};