#include "Coins.h"
#include "Game.h"
#include "GameApp.h"
#include "Sprite.h"

Coins::Coins(const Sprite & sprite)
	:mSprite(sprite)
{
	mSprite = sprite;
	mCoins = 0;
}

Coins::~Coins()
{
}

void Coins::addCoins(int amount)
{
	mCoins += amount;
}

void Coins::draw()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	for (int i = 0; i < mCoins; i++)
	{
		Unit* pUnit = pGame->getUnitManager()->createRandomObject(mSprite);
		if (pUnit == NULL)
		{
			pGame->getUnitManager()->deleteRandomUnit();
		}
	}
}
