#pragma once

#include <Trackable.h>
#include "Sprite.h"
#include "UnitManager.h"
#include "GameApp.h"
#include "Game.h"

class GraphicsBuffer;

class AllMightyCandy :public Trackable
{
public:
	AllMightyCandy(const Sprite& sprite);
	~AllMightyCandy();

	void draw();
	void update();

	void setID(int myID) { mID = myID; };
	int getID() { return mID; };

private:
	int mCoins;
	Sprite mSprite;
	int mID;
	int timer = 0;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
};