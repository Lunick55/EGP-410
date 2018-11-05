#include "WanderAndChaseSteering.h"
#include "Unit.h"
#include "Game.h"
#include "UnitManager.h"

WanderAndChaseSteering::WanderAndChaseSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering(),
	mWander(WanderSteering(ownerID, targetLoc, mTargetID)),
	mFace(FaceSteering(ownerID, targetLoc, mTargetID)),
	mSeek(SeekSteering(ownerID, targetLoc, mTargetID))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::WANDER_AND_CHASE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering * WanderAndChaseSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Vector2D myPlayerLocation = gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
	Vector2D diff = myPlayerLocation - pOwner->getPositionComponent()->getPosition();


	Steering* mySteer = NULL;

	if (diff.getLengthSquared() > mWanderRadius * mWanderRadius)
	{
		mySteer = mWander.getSteering();
		data.rotAcc = mySteer->getData().rotAcc;
	}
	else
	{
		mSeek.setTargetLoc(myPlayerLocation);
		mySteer = mSeek.getSteering();
		mFace.setTargetLoc(myPlayerLocation);
		data.rotAcc = mFace.getSteering()->getData().rotAcc;
	}

	data.acc = mySteer->getData().acc;

	this->mData = data;
	return this;
}
