#include <cassert>

#include "Steering.h"
#include "ArriveAndFace.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveAndFace::ArriveAndFace(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering(), mArriveSteering(ArriveSteering(ownerID, targetLoc, targetID, shouldFlee)),
mFaceSteering(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))

{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::ARRIVE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* ArriveAndFace::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mFaceSteering.getSteering();
	mArriveSteering.getSteering();

	data.rotAcc = mFaceSteering.getData().rotAcc;
	data.rotVel = mFaceSteering.getData().rotVel;

	data.acc = mArriveSteering.getData().acc;
	data.vel = mArriveSteering.getData().vel;

	

	this->mData = data;
	return this;
}