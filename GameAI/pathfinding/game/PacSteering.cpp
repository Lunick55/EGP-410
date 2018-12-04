#include "PacSteering.h"
#include "ArriveAndFaceSteering.h"
#include "Graph.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "GraphicsSystem.h"

PacSteering::PacSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering(), mPlayerSteering(Player(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::FOLLOW_PATH;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	index = 0;
}

void PacSteering::setPath(Path* myPath)
{
	mPath = *myPath;
}

void PacSteering::resetIndex()
{
	index = 0;
}


Steering * PacSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	//draw the path for each of the arrows
	//pGame->getPathfinder()->drawPath(pGame->getGrid(), pGame->getGraphicsSystem()->getBackBuffer() , mPath);

	Vector2D direction;
	float distance;
	float targetRadius = 30.0f;

	//get the target location for each of the nodes
	direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	distance = direction.getLength();

	//As long as we havent gotten to the end node
	if (distance < targetRadius && mPath.peekNode(index) != NULL)
	{
		//get the grid and set the target to arrive and face so that it can move to it
		Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
		Vector2D target = pGrid->getULCornerOfSquare(mPath.peekNode(index)->getId());
		mPlayerSteering.setTargetLoc(target);
		setTargetLoc(target);
		//increase the index for the next node
		index++;
	}

	//now do stuff
	Steering* mPlayerSteer = mPlayerSteering.getSteering();

	//set the acceleration, velocity, and rotationacceleration to what arrive and face have declared it to be
	data.vel = mPlayerSteer->getData().vel;

	//return the data
	this->mData = data;

	return this;
}

