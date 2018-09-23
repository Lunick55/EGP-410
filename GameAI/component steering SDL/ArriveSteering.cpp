#include <cassert>

#include "Steering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
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

Steering* ArriveSteering::getSteering()
{
	float stopRadius = 0.2;
	float slowRadius = 150;
	float timeToTarget = 0.1f;

	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	//----------Dynamic attempt
	if (diff.getLength() < stopRadius)
	{
		data.acc = 0;
		data.vel= 0;
		this->mData = data;
		return this;
	}

	float targetSpeed;
	if (diff.getLength() > slowRadius)
	{
		targetSpeed = pOwner->getMaxSpeed();
	}
	else
	{
		targetSpeed = pOwner->getMaxSpeed() * diff.getLength() / slowRadius;
	}
	
	Vector2D targetVelocity;
	targetVelocity = diff;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;

	data.acc = targetVelocity - data.vel; //pOwner->getPhysicsComponent()->getVelocity();
	data.acc /= timeToTarget;

	if (data.acc.getLength() > pOwner->getMaxAcc())
	{
		data.acc.normalize();
		data.acc *= pOwner->getMaxAcc();
	}

	//data.acc = diff;

	//float velocityDirection = atan2(diff.getY(), diff.getX()) + (3.14159 / 2);
	//pOwner->getPositionComponent()->setFacing(velocityDirection);

	//data.rotVel = 1.0f;
	this->mData = data;
	return this;
}

