#pragma once

#include <Trackable.h>
#include "Sprite.h"
#include "UnitManager.h"
#include "Vector2D.h"
#include "Timer.h"

class GraphicsBuffer;

class Player :public Trackable
{
public:
	Player(const Sprite& sprite);
	~Player();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void update(double dt);

	Vector2D getPosition();

	void draw();

private:
	Sprite mSprite;
	int mPosX;
	int mPosY;
	double mSpeed;
	bool isUnitDown, isUnitRight, isUnitLeft, isUnitUp;
	Timer mTimer;

};