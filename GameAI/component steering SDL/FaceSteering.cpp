#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::FACE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* FaceSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	float targetAngle = 0.01;
	float slowAngle = 0.5;
	float timeToTarget = 0.1;

	/*if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}*/

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (diff.getLength() == 0)
	{
		//Do nothing?
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}

	float targetOrient = atan2(diff.getY(), diff.getX()) + (3.14159 / 2);
	float rotation = targetOrient - pOwner->getFacing();
	float rotationDirection;
	rotation = fmod(rotation, (2 * 3.14f));// - 3.14f;
	if (rotation > 3.14f)
	{
		rotation - 3.14f;
		rotation *= -1.0f;
	}
	if (rotation < -3.14f)
	{
		rotation + 3.14f;
		rotation *= -1.0f;
	}

	float rotationSize = abs(rotation);

	float targetRotation;

	if (rotationSize < targetAngle)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}
	if (rotationSize > slowAngle)
	{
		targetRotation = data.maxRotAcc;
	}
	else
	{
		targetRotation = data.maxRotAcc * rotationSize / slowAngle;
	}

	targetRotation *= rotation / rotationSize;
	


	data.rotAcc = targetRotation - data.rotVel;//pOwner->getPhysicsComponent()->getRotationalVelocity();
	data.rotAcc /= timeToTarget;

	float angularAcc = abs(data.rotAcc);

	if (angularAcc > data.maxRotAcc)
	{
		data.rotAcc /= angularAcc;
		data.rotAcc *= data.maxRotAcc;
	}

	data.acc = 0;
	this->mData = data;
	
	return this;
}

