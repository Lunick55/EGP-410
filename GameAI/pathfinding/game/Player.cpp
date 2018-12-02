#include "Player.h"
#include "GameApp.h"
#include "PositionComponent.h"

Player::Player(const Sprite & sprite)
	:mSprite(sprite)
{
	mSpeed = 300.0;
	mPosX = 256;
	mPosY = 320;
	isUnitDown = false; 
	isUnitRight = false;
	isUnitLeft = false;
	isUnitUp = false;
	mTimer.start();
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
	isUnitUp = false;
	isUnitLeft = true;
	isUnitDown = false;
	isUnitRight = false;
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
	isUnitUp = false;
	isUnitLeft = false;
	isUnitDown = false;
	isUnitRight = true;
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
	isUnitUp = true;
	isUnitLeft = false;
	isUnitDown = false;
	isUnitRight = false;
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
	isUnitUp = false;
	isUnitLeft = false;
	isUnitDown = true;
	isUnitRight = false;
	//PositionData(Vector2D(mPosX, mPosY), 0);
}

void Player::update(double dt)
{
	if (mTimer.getElapsedTime() > mSpeed)
	{
		if (isUnitDown)
		{
			moveDown();
		}
		else if (isUnitLeft)
		{
			moveLeft();
		}
		else if (isUnitRight)
		{
			moveRight();
		}
		else if (isUnitUp)
		{
			moveUp();
		}
		mTimer.start();
	}
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

