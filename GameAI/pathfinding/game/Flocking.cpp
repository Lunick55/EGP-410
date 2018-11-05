#include "Flocking.h"
#include "Unit.h"
#include "UnitManager.h"
#include "Game.h"
#include "WanderSteering.h"

Flocking::Flocking(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	:Steering(),
	mCohesion(Cohesion(ownerID, targetLoc, mTargetID, false)),
	mAlignment(GroupAlignment(ownerID, targetLoc, mTargetID, false)),
	mSeperation(Seperation(ownerID, targetLoc, mTargetID, false)),
	mWander(WanderSteering(ownerID, targetLoc, mTargetID, false))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::FLOCK;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

}


Steering * Flocking::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D mTempCohesion = mCohesion.getCohesion();
	Vector2D mTempAlign = mAlignment.getAlignment();
	Vector2D mTempSeperate = mSeperation.getSeperation();
	Steering* mTempWander = mWander.getSteering();

	float x, y;

	//get units to wander
	data = mTempWander->getData();

	//update the blended weights
	updateBoidWeight();

	//assign blended weights to float variables
	x = (mTempAlign.getX() * mGroupAlignmentWeight) + (mTempCohesion.getX() * mCohesionWeight) + (mTempSeperate.getX() * mSeperationWeight);
	y = (mTempAlign.getY() * mGroupAlignmentWeight) + (mTempCohesion.getY() * mCohesionWeight) + (mTempSeperate.getY() * mSeperationWeight);

	//set blended weights inside vector and then normalize them
	Vector2D blendedWeights = Vector2D(x, y);
	blendedWeights.normalize();

	//get the orientation of the units and normalize them
	Vector2D mapOrientation(cos(pOwner->getFacing() * mRadius), sin(pOwner->getFacing() * mRadius));
	mapOrientation.normalize();

	//set the direction that they currently are in
	mDirection.setX(cos(mRotation * 3.14f / 180.0f) * mRadius);
	mDirection.setY(sin(mRotation * 3.14f / 180.0f) * mRadius);

	//set the target direction and add the blended weights to it
	mTargetDirection = mDirection + (mapOrientation * mOffset);
	mTargetDirection.normalize();
	mTargetDirection += blendedWeights;

	//set the facing of the units
	FaceSteering mFace(mOwnerID, mTargetDirection + pOwner->getPositionComponent()->getPosition(), mTargetID, false);
	data = mFace.getSteering()->getData();

	//normalize the direction and assign it to the untis
	mTargetDirection.normalize();
	data.acc = mTargetDirection * MAX_ACC;

	this->mData = data;
	return this;
}

void Flocking::updateBoidWeight()
{
	//update the weights
	mGroupAlignmentWeight = gpGame->getAlignmentWeight();
	mCohesionWeight = gpGame->getCohesionWeight();
	mSeperationWeight = gpGame->getSeperationWeight();

	//cap the weights at 0.0 and 1.0f
	if (mGroupAlignmentWeight > 1.0f)
	{
		gpGame->setAlignmentWeight(1.0f);
		mGroupAlignmentWeight = 1.0f;
	}
	else if (mCohesionWeight > 1.0f)
	{
		gpGame->setCohesionWeight(1.0f);
		mCohesionWeight = 1.0f;
	}
	else if (mSeperationWeight > 1.0f)
	{
		gpGame->setSeperationWeight(1.0f);
		mSeperationWeight = 1.0f;
	}

	if (mGroupAlignmentWeight < 0.0f)
	{
		gpGame->setAlignmentWeight(0.0f);
		mGroupAlignmentWeight = 0.0f;
	}
	else if (mCohesionWeight  < 0.0f)
	{
		gpGame->setCohesionWeight(0.0f);
		mCohesionWeight = 0.0f;
	}
	else if (mSeperationWeight  < 0.0f)
	{
		gpGame->setSeperationWeight(0.0f);
		mSeperationWeight = 0.0f;
	}
}