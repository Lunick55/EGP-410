#include "Player.h"
#include "GameApp.h"
#include "PositionComponent.h"
#include "Grid.h"
#include "Node.h"

Player::Player(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	:Steering()
{
	setOwnerID(ownerID);
	init();
}

void Player::init()
{
	mSpeed = 10.0;
	mPosX = 256;
	mPosY = 320;
	mMovement = 20;
	isUnitDown = false;
	isUnitRight = false;
	isUnitLeft = false;
	isUnitUp = false;
	mTimer.start();
}
Player::~Player()
{
}

Steering * Player::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Vector2D currPos = pOwner->getPositionComponent()->getPosition();

	/*
	KELLY
	Yo, so the movement works in this big tree. The thing is, if you wanna do collision checks,
	they gotta happen here. Check wall doesn't super work the way I have it implemented here.

	Because it's velocity based now, we can't just check 'x' pixels ahead of it. 

	What we need to do is find a way of checking if the NEXT node along the current path is 
	a wall, and if so, stop at it, not before. 

	The checking of the next wall can be done however you want. Raycast, a pixelAtIndex thing,
	whatever, I dunno. But to stop AT the wall and NOT BEFORE we could get the pixel edge of it,
	then stop the velocity at that edge. Again, if you find an efficient way, props to you. 

	GLHF
	
	*/
	if (isUnitLeft)
	{
		if (!pGame->getUnitManager()->checkWall(currPos.getX() - mMovement, currPos.getY()))
		{
			pOwner->getPositionComponent()->setFacing(PI);
			data.vel = Vector2D(-mMovement, 0);
		}
	}
	else if (isUnitRight)
	{
		if (!pGame->getUnitManager()->checkWall(currPos.getX() + mMovement, currPos.getY()))
		{
			pOwner->getPositionComponent()->setFacing(0);
			data.vel = Vector2D(mMovement, 0);
		}
	}
	else if (isUnitUp)
	{
		if (!pGame->getUnitManager()->checkWall(currPos.getX(), currPos.getY() - mMovement))
		{
			pOwner->getPositionComponent()->setFacing(3 * PI / 2);
			data.vel = Vector2D(0, -mMovement);
		}
	}
	else if (isUnitDown)
	{
		if (!pGame->getUnitManager()->checkWall(currPos.getX(), currPos.getY() + mMovement))
		{
			pOwner->getPositionComponent()->setFacing(PI / 2);
			data.vel = Vector2D(0, mMovement);
		}
	}
	else data.vel = 0;

	this->mData = data;

	return this;
}

void Player::moveLeft()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);




	isUnitUp = false;
	isUnitLeft = true;
	isUnitDown = false;
	isUnitRight = false;
}

void Player::moveRight()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	//if (!pGame->getUnitManager()->checkWall(mPosX, mPosY))
	//{

	//}

	isUnitUp = false;
	isUnitLeft = false;
	isUnitDown = false;
	isUnitRight = true;
}

void Player::moveUp()
{
	//Vector2D diff;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	//mPosY -= mMovement;
	//if (pGame->getUnitManager()->checkWall(mPosX, mPosY - mMovement))
	//{
	//	mPosY -= mMovement;
	//	pGame->getUnitManager()->updatePacman(mSprite, mPosX, mPosY);
	//}
	//else
	//{
	//	mPosY += mMovement;
	//}
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
	//mPosY += mMovement;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	//if (pGame->getUnitManager()->checkWall(mPosX, mPosY+mMovement))
	//{
	//	mPosY += mMovement;
	//	pGame->getUnitManager()->updatePacman(mSprite, mPosX, mPosY);
	//}
	//else
	//{
	//	mPosY -= mMovement;
	//}
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
	//GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	//Unit* pUnit = pGame->getUnitManager()->createPacman(mSprite);
	//if (pUnit == NULL)
	//{
	//	pGame->getUnitManager()->deleteRandomUnit();
	//}
	
}

bool Player::rayClear(Node * outputPath, Node * inputPath)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);


	if (pGame != NULL)
	{
		Grid *pGrid = pGame->getGrid();

		Vector2D from = pGrid->getULCornerOfSquare(outputPath->getId());
		Vector2D to = pGrid->getULCornerOfSquare(inputPath->getId());

		Vector2D diff;

		//get the direction from the input path and the output path
		diff = to - from;

		int diffY = (int)diff.getY();
		int diffX = (int)diff.getX();

		//gets the number of checks between input/output path
		int distance = sqrt((diffX * diffX) + (diffY * diffY));
		int amountChecks = 50;

		int posX = (int)from.getX();
		int posY = (int)from.getY();

		//continues in between each node in order to see if there is a wall
		for (int i = 0; i < amountChecks; i++)
		{
			//if there is a wall
			if (pGame->getGrid()->getValueAtPixelXY((int)posX, (int)posY) == BLOCKING_VALUE)
			{
				return false;
			}
			//update the position
			posX = posX + (diffX / amountChecks);
			posY = posY + (diffY / amountChecks);
		}
		//we have found that there is no wall!
		return true;
	}
	return false;
}


