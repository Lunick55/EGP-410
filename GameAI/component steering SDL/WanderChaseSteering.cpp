#include <cassert>

#include "Steering.h"
#include "WanderChaseSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


WanderChaseSteering::WanderChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	:Steering(), mWanderSteering(WanderSteering(ownerID, targetLoc, targetID, shouldFlee)), mSeekSteering(SeekSteering(ownerID, targetLoc, targetID, shouldFlee)), mFaceSteering(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEEK;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* WanderChaseSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}


	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	if (diff.getLength() > mSeekRadius)
	{
		Steering* tempWander = mWanderSteering.getSteering();
		data = tempWander->getData();	
	}
	else 
	{
		//Get seekSteering
		mSeekSteering.getSteering();
		data = mSeekSteering.getData();//tempSeek->getData();

		//Get faceSteering
		mFaceSteering.setTargetLoc(mTargetLoc);
		mFaceSteering.getSteering();

		data.rotAcc = mFaceSteering.getData().rotAcc;//tempFace->getData().rotAcc;		
		data.rotVel = mFaceSteering.getData().rotVel;//tempFace->getData().rotVel;
	}

	this->mData = data;
	return this;
}
