#include "FollowPath.h"
#include "ArriveAndFaceSteering.h"
#include "Graph.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "GraphicsSystem.h"

FollowPath::FollowPath(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering()
	, mArriveAndFaceSteering(ArriveAndFaceSteering(ownerID, targetLoc, targetID,shouldFlee))
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

	mPath;// = new Path();

}

void FollowPath::setPath(Path* myPath)
{
	mPath = *myPath;
}

void FollowPath::resetIndex()
{
	index = 0;
}


Steering * FollowPath::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	pGame->getPathfinder()->drawPath(pGame->getGrid(), pGame->getGraphicsSystem()->getBackBuffer() , mPath);

	if (data.acc == 0 && mPath.peekNode(index) != NULL)
	{
		Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
		Vector2D target = pGrid->getULCornerOfSquare(mPath.peekNode(index)->getId());
		mArriveAndFaceSteering.setTargetLoc(target);
		setTargetLoc(target);
		index++;
	}

//now do stuff
	Steering* mArriveFaceSteer = mArriveAndFaceSteering.getSteering();

	data.acc = mArriveFaceSteer->getData().acc;
	data.vel = mArriveFaceSteer->getData().vel;
	data.rotAcc = mArriveFaceSteer->getData().rotAcc;

	this->mData = data;

	return this;
}


