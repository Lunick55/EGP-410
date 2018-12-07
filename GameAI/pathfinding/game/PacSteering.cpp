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
	
	int xVal = ((16 * mSpeedX) / 2) + mSpeedX;
	int yVal = ((16 * mSpeedY) / 2) + mSpeedY;

	mSquareIndexShifted = pGrid->getSquareIndexFromPixelXY(mPositionCentered.getX() + xVal, mPositionCentered.getY() + yVal);
	mSquareIndexAtCenter = pGrid->getSquareIndexFromPixelXY(mPositionCentered.getX(), mPositionCentered.getY());
	if (pGrid->getValueAtIndex(mSquareIndexAtCenter) == INTERSECTION_VALUE)
	{
		cout << "I'M HOME" << endl;
		pGame->SetPacCanMove(true);
	}
	if (pGrid->getValueAtIndex(mSquareIndexShifted) == BLOCKING_VALUE)
	{
		cout << "I'M BACKED AGAINST THE WALL" << endl;
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
		mSpeedX = -2;
		mSpeedY = 0;
		newDir = "left";
		owner->getPositionComponent()->setFacing(PI);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitRight)
	{
		mSpeedX = 2;
		mSpeedY = 0;
		newDir = "right";
		owner->getPositionComponent()->setFacing(0);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitUp)
	{
		mSpeedX = 0;
		mSpeedY = -2;
		newDir = "up";
		owner->getPositionComponent()->setFacing(3 * PI / 2);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitDown)
	{
		mSpeedX = 0;
		mSpeedY = 2;
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
		cout << "I'M HOME" << endl;
		pOwner->getPositionComponent()->setPosition(pGrid->getULCornerOfSquare(mSquareIndexAtCenter));
	}
}