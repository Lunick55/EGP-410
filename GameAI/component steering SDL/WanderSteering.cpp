#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: mWanderOrientation(0), Steering(), mFaceSteering(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
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

Steering* WanderSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	//forward offset and radius of wander
	float wanderOffset = 100.0f;
	float wanderRadius = 30.0f;

	//maximum rate at which the wanOrien can change
	float wanderRate = 0.8;
	
	//update wander orientation
	mWanderOrientation += (genRandomBinomial() * wanderRate);

	//calculate combined target Orient
	float targetOrientation;
	targetOrientation = mWanderOrientation + (pOwner->getFacing() - (3.14159 / 2));

	//calculate center of wander cirlce
	Vector2D target;
	target = pOwner->getPositionComponent()->getPosition() + (asVector(pOwner->getFacing() - (3.14159 / 2)) * wanderOffset);
	cout << (pOwner->getFacing() - (3.14159 / 2)) * wanderOffset << endl; //----------

	//calculate target location
	target += (asVector(targetOrientation) * wanderRadius);
	cout << targetOrientation * wanderRadius << endl; //--------------

	cout << diff.getX() << endl;

	//do a facing delegate to FACE using target
	mFaceSteering.setTargetLoc(target);
	Steering* tempFace = mFaceSteering.getSteering();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data.rotAcc = tempFace->getData().rotAcc;
	data.rotVel = tempFace->getData().rotVel;
	//cout << "ROTACC " << data.rotAcc << endl;
	//cout << "TARORT " << targetOrientation << endl;
	//cout << "WANORT " << mWanderOrientation << endl;
	//cout << "TARGTX " << target.getX() << endl;
	//cout << "TARGTY " << target.getY() << endl;

	
	data.acc = asVector(pOwner->getFacing() - (3.14159 / 2)) * pOwner->getMaxAcc();

	this->mData = data;
	return this;
}

Vector2D WanderSteering::asVector(float num)
{
	Vector2D temp;
	float x = cos(num);
	float y = sin(num);

	temp.setX(x);
	temp.setY(y);

	return temp;
}