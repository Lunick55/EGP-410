#include "Player.h"
#include "GameApp.h"

Player::Player(const Sprite & sprite)
	:mSprite(sprite)
{
}

Player::~Player()
{
}

void Player::draw()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pUnit = pGame->getUnitManager()->createPacman(mSprite);
	if (pUnit == NULL)
	{
		pGame->getUnitManager()->deleteRandomUnit();
	}
	
}

