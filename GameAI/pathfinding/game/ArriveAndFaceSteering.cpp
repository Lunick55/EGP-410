#include "ArriveAndFaceSteering.h"
#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cassert>

ArriveAndFaceSteering::ArriveAndFaceSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering()
	, mArriveSteering(ArriveSteering(ownerID, targetLoc, targetID, shouldFlee)),
	mFaceSteering(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::ARRIVE_AND_FACE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}
Steering * ArriveAndFaceSteering::getSteering()
{

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Steering* mFaceSteer = mFaceSteering.getSteering();
	Steering* mArriveSteer = mArriveSteering.getSteering();

	if (mArriveSteer != NULL)
	{
		data.acc = mArriveSteer->getData().acc;
		if (mFaceSteer != NULL)
		{
			data.rotAcc = mFaceSteer->getData().rotAcc;
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
