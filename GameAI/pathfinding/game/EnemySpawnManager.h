#pragma once

#include <Trackable.h>
#include "Sprite.h"
#include "UnitManager.h"


class EnemySpawnManager :public Trackable
{
public:
	EnemySpawnManager(const Sprite& sprite);
	~EnemySpawnManager();

	void draw();

private:


};