#pragma once

#include <Trackable.h>
#include "Sprite.h"
#include "UnitManager.h"
#include "Vector2D.h"

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

	Vector2D getPosition();

	void draw();

private:
	Sprite mSprite;
	int mPosX;
	int mPosY;
	double mSpeed;

};