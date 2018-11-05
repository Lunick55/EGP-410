#include <cassert>
#include "FaceSteering.h"
#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <math.h>

FaceSteering::FaceSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
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

float FaceSteering::MapToRange(float rotation)
{
	float convertRot;

	convertRot = fmod((rotation), 2.0 * PIE);
	if (convertRot > PIE)
	{
		convertRot - PIE;
		convertRot *= -1.0;

	}
	else if (convertRot < -PIE)
	{
		convertRot + PIE;
		convertRot *= -1.0;
	}
	return convertRot;
}

Steering * FaceSteering::getSteering()
{
	Vector2D direction;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	if (mType == Steering::FACE)
	{
		direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		direction = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	myAngle = (atan2(direction.getY(), direction.getX()) + (PIE / 2)) - pOwner->getFacing();
	//myAngle = fmod(myAngle, 3.14159f * 2);
	myAngle = MapToRange(myAngle);

	rotationSize = abs(myAngle);



	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//check if they are there
	if (rotationSize < mTargetRadius)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}

	if (rotationSize > mSlowRadius)
	{
		targetRotation = data.maxRotAcc;
	}
	else
	{
		targetRotation = data.maxRotAcc * rotationSize / mSlowRadius;
	}

	targetRotation *= myAngle / rotationSize;

	//steering.angular
	data.rotAcc = targetRotation - data.rotVel; //<- character.rotation
	data.rotAcc /= mTimeToTarget;

	float angularAcceleration = abs(data.rotAcc);

	if (angularAcceleration > data.maxRotAcc)
	{
		data.rotAcc /= angularAcceleration;
		data.rotAcc *= data.maxRotAcc;
	}
	//steering.linear
	data.acc = 0;
	this->mData = data;
	return this;
}
