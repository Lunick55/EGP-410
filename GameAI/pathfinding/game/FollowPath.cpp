#include "FollowPath.h"
#include "ArriveAndFaceSteering.h"
#include "Graph.h"
#include "GameApp.h"
#include "UnitManager.h"

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
}

void FollowPath::setPath(Path* myPath)
{
	mPath = *myPath;
}

Steering * FollowPath::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();


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

	if (mArriveFaceSteer != NULL)
	{
		data.acc = mArriveFaceSteer->getData().acc;
		if (mArriveFaceSteer != NULL)
		{
			data.rotAcc = mArriveFaceSteer->getData().rotAcc;
		}
	}
	else
	{
		data.rotAcc = 0;
		data.acc = 0;
		data.vel = 0;
		data.rotVel = 0;
	}

	this->mData = data;

	return this;
}
