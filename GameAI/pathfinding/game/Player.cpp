#include "Player.h"
#include "GameApp.h"
#include "PositionComponent.h"

Player::Player(const Sprite & sprite)
	:mSprite(sprite)
{
	mSpeed = 300.0;
	mPosX = 256;
	mPosY = 320;
}

Player::~Player()
{
}

void Player::moveLeft()
{
	mPosX -= 32;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame->getUnitManager()->checkDirection(mPosX, mPosY))
	{
		pGame->getUnitManager()->updatePacman(mSprite, mPosX, mPosY);
	}
	else
	{
		mPosX += 32;
	}
	//PositionData(Vector2D(mPosX, mPosY), 0);
}

void Player::moveRight()
{
	mPosX += 32;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame->getUnitManager()->checkDirection(mPosX, mPosY))
	{
		pGame->getUnitManager()->updatePacman(mSprite, mPosX, mPosY);
	}
	else
	{
		mPosX -= 32;
	}
	
	//PositionData(Vector2D(mPosX, mPosY), 0);
}

void Player::moveUp()
{
	//Vector2D diff;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	mPosY -= 32;
	if (pGame->getUnitManager()->checkDirection(mPosX, mPosY))
	{
		pGame->getUnitManager()->updatePacman(mSprite, mPosX, mPosY);
	}
	else
	{
		mPosY += 32;
	}
	//draw();
	//Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	//PositionData(Vector2D(mPosX, mPosY), 0);

}

void Player::moveDown()
{
	mPosY += 32;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame->getUnitManager()->checkDirection(mPosX, mPosY))
	{
		pGame->getUnitManager()->updatePacman(mSprite, mPosX, mPosY);
	}
	else
	{
		mPosY -= 32;
	}
	//PositionData(Vector2D(mPosX, mPosY), 0);
}

Vector2D Player::getPosition()
{
	return Vector2D(mPosX, mPosY);
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

