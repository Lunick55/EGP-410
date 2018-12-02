#pragma once

#include <Trackable.h>
#include "Sprite.h"
#include "UnitManager.h"

class GraphicsBuffer;

class Player :public Trackable
{
public:
	Player(const Sprite& sprite);
	~Player();

	void draw();

private:
	Sprite mSprite;

};