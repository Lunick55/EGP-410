#pragma once

#include <Trackable.h>
#include "Sprite.h"
#include "UnitManager.h"
#include "GameApp.h"
#include "Game.h"

class GraphicsBuffer;

class Powerup :public Trackable
{
public:
	Powerup(const Sprite& sprite);
	~Powerup();

	void draw();
	void update(float elapsedTime);

	void reset();

	void setID(int myID) { mID = myID; };
	int getID() { return mID; };

	void setEnemyID(int myID) { enemyID = myID; };
	int getEnemyID() { return enemyID; };

private:
	int mCoins;
	Sprite mSprite;
	int mID;
	int enemyID;
	float timer = 0;
	bool increaseTime = false;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
};