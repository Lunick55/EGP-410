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

	Vector2D direction;

	//get the target location for each of the nodes
	direction = (mTargetLoc + Vector2D(16, 16)) - (pOwner->getPositionComponent()->getPosition() + Vector2D(16,16));
	//direction = (mTargetLoc - pOwner->getPositionComponent()->getPosition());

	if (mPath.peekNode(mIndex) != NULL)
	{
		//get the grid and set the target to arrive and face so that it can move to it
		Vector2D target = pGrid->getULCornerOfSquare(mPath.peekNode(mIndex)->getId());
		//mPlayerSteering.setTargetLoc(target);
		setTargetLoc(target);
		//increase the index for the next node
		mIndex++;
	}
	
	//now do stuff
	checkDirection(pOwner);

	Vector2D positionCentered = pOwner->getPositionComponent()->getPosition() + Vector2D(16,16);

	//As long as we havent gotten to the end node
	
	int xVal = ((16 * mSpeedX) / 2) + mSpeedX;
	int yVal = ((16 * mSpeedY) / 2) + mSpeedY;

	int squareIndexShifted = pGrid->getSquareIndexFromPixelXY(positionCentered.getX() + xVal, positionCentered.getY() + yVal);
	int squareIndexAtCenter = pGrid->getSquareIndexFromPixelXY(positionCentered.getX(), positionCentered.getY());
	if (pGrid->getValueAtIndex(squareIndexAtCenter) == INTERSECTION_VALUE)
	{
		cout << "I'M HOME" << endl;
		//pOwner->getPositionComponent()->setPosition(mTargetLoc);
		pGame->SetPacCanMove(true);
	}
	if (pGrid->getValueAtIndex(squareIndexShifted) == BLOCKING_VALUE)
	{
		cout << "I'M BACKED AGAINST THE WALL" << endl;
		pOwner->getPositionComponent()->setPosition(mTargetLoc);
		pGame->SetPacCanMove(false);
	}

	//if (mPath.peekNode(index) == NULL)
	//{
	//	//cout << "STTOOOOOPPPP" << endl;
	//	data.vel = 0;
	//	pOwner->getPositionComponent()->setPosition(mTargetLoc);
	//}
	//return the data
	this->mData = data;

	return this;
}

void PacSteering::checkDirection(Unit* owner)
{
	if (isUnitLeft)
	{
		mSpeedX = -2;
		mSpeedY = 0;

		owner->getPositionComponent()->setFacing(PI);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
		//data.vel = Vector2D(-mMovement, 0);
	}
	else if (isUnitRight)
	{
		mSpeedX = 2;
		mSpeedY = 0;

		owner->getPositionComponent()->setFacing(0);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
		//data.vel = Vector2D(mMovement, 0);

	}
	else if (isUnitUp)
	{
		mSpeedX = 0;
		mSpeedY = -2;

		owner->getPositionComponent()->setFacing(3 * PI / 2);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));

		//data.vel = Vector2D(0, -mMovement);
	}
	else if (isUnitDown)
	{
		mSpeedX = 0;
		mSpeedY = 2;

		owner->getPositionComponent()->setFacing(PI / 2);
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));

		//data.vel = Vector2D(0, mMovement);
	}
}

void PacSteering::moveDirection(Vector2D directionX, Vector2D directionY)
{
	isUnitLeft = directionX.getX();
	isUnitRight = directionX.getY();
	isUnitUp = directionY.getX();
	isUnitDown = directionY.getY();
}