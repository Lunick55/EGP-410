#include "PacSteering.h"
#include "ArriveAndFaceSteering.h"
#include "Graph.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "GraphicsSystem.h"

PacSteering::PacSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering()//, mPlayerSteering(Player(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::PAC_STEER;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	isUnitDown = false;
	isUnitRight = false;
	isUnitLeft = false;
	isUnitUp = false;

	mSpeed = 2.0;
	mSpeedX = mSpeedY = 0;

	mIndex = 0;
}

void PacSteering::setPath(Path* myPath)
{
	mPath = *myPath;
}

void PacSteering::resetIndex()
{
	mIndex = 0;
}


Steering * PacSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();

	//get the target location for each of the nodes
	if (mPath.peekNode(mIndex) != NULL)
	{
		//get the grid and set the target to arrive and face so that it can move to it
		Vector2D target = pGrid->getULCornerOfSquare(mPath.peekNode(mIndex)->getId());

		setTargetLoc(target);
		//increase the index for the next node
		mIndex++;
	}
	
	//now do stuff
	checkDirection(pOwner);

	mPositionCentered = pOwner->getPositionComponent()->getPosition() + Vector2D(16,16);

	//As long as we havent gotten to the end node
	
	int xVal = ((16 * mSpeedX) / mSpeed) + mSpeedX;
	int yVal = ((16 * mSpeedY) / mSpeed) + mSpeedY;

	mSquareIndexAtCenter = pGrid->getSquareIndexFromPixelXY(mPositionCentered.getX(), mPositionCentered.getY());
	mSquareIndexShifted = pGrid->getSquareIndexFromPixelXY(mPositionCentered.getX() + xVal, mPositionCentered.getY() + yVal);
	if (pGrid->getValueAtIndex(mSquareIndexAtCenter) == INTERSECTION_VALUE)
	{
		//pGame->SetPacCanMove(true);
	}
	if (pGrid->getValueAtIndex(mSquareIndexShifted) == BLOCKING_VALUE)
	{
		pOwner->getPositionComponent()->setPosition(mTargetLoc);
	}

	if (newDir != oldDir)
	{
		centerToNode();
	}

	oldDir = newDir;

	this->mData = data;

	return this;
}

void PacSteering::checkDirection(Unit* owner)
{
	if (isUnitLeft)
	{
		mSpeedX = -mSpeed;
		mSpeedY = 0;
		newDir = "left";
		owner->getPositionComponent()->setFacing(PI);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitRight)
	{
		mSpeedX = mSpeed;
		mSpeedY = 0;
		newDir = "right";
		owner->getPositionComponent()->setFacing(0);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitUp)
	{
		mSpeedX = 0;
		mSpeedY = -mSpeed;
		newDir = "up";
		owner->getPositionComponent()->setFacing(3 * PI / 2);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitDown)
	{
		mSpeedX = 0;
		mSpeedY = mSpeed;
		newDir = "down";
		owner->getPositionComponent()->setFacing(PI / 2);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
}

void PacSteering::moveDirection(Vector2D directionX, Vector2D directionY)
{
	isUnitLeft = directionX.getX();
	isUnitRight = directionX.getY();
	isUnitUp = directionY.getX();
	isUnitDown = directionY.getY();
}

void PacSteering::centerToNode()
{
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);

	mPositionCentered = pOwner->getPositionComponent()->getPosition() + Vector2D(16, 16);

	mSquareIndexAtCenter = pGrid->getSquareIndexFromPixelXY(mPositionCentered.getX(), mPositionCentered.getY());
	if (pGrid->getValueAtIndex(mSquareIndexAtCenter) == INTERSECTION_VALUE)
	{
		
		pOwner->getPositionComponent()->setPosition(pGrid->getULCornerOfSquare(mSquareIndexAtCenter));
	}
}