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
	mSpeed = 32.0;
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

	if (isUnitLeft)
	{
		pOwner->getPositionComponent()->setFacing(PI);
		data.vel = Vector2D(-mMovement, 0);
	}
	else if (isUnitRight)
	{

		pOwner->getPositionComponent()->setFacing(0);
		data.vel = Vector2D(mMovement, 0);
		
	}
	else if (isUnitUp)
	{

		pOwner->getPositionComponent()->setFacing(3 * PI / 2);
		data.vel = Vector2D(0, -mMovement);
	}
	else if (isUnitDown)
	{
		pOwner->getPositionComponent()->setFacing(PI / 2);
		data.vel = Vector2D(0, mMovement);
	
	}
	else data.vel = 0;

	this->mData = data;

	return this;
}

void Player::moveLeft()
{
	isUnitUp = false;
	isUnitLeft = true;
	isUnitDown = false;
	isUnitRight = false;
}

void Player::moveRight()
{
	isUnitUp = false;
	isUnitLeft = false;
	isUnitDown = false;
	isUnitRight = true;
}

void Player::moveUp()
{

	isUnitUp = true;
	isUnitLeft = false;
	isUnitDown = false;
	isUnitRight = false;
}

void Player::moveDown()
{
	isUnitUp = false;
	isUnitLeft = false;
	isUnitDown = true;
	isUnitRight = false;
}

void Player::moveDirection(Vector2D directionX, Vector2D directionY)
{
	isUnitLeft = directionX.getX();
	isUnitRight = directionX.getY();
	isUnitUp = directionY.getX();
	isUnitDown = directionY.getY();
}

Vector2D Player::getPosition()
{
	return Vector2D(mPosX, mPosY);
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


