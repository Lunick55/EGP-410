#include <cassert>
#include "WanderSteering.h"
#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "GameApp.h"
#include <math.h>

WanderSteering::WanderSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering()
	, mFaceSteering(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::WANDER;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Vector2D WanderSteering::to2DVector(float convert)
{
	Vector2D temp;
	temp.setX(cos(convert));
	temp.setY(sin(convert));
	return temp;
}

Steering * WanderSteering::getSteering()
{
	Vector2D diff;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData owner = pOwner->getPhysicsComponent()->getData();
	float wanderOffset = 100.0f;
	float wanderRadius = 30.0f;
	mWanderRate = 0.8f;

	//need to allow for negative orientation
	mWanderOrientation += (genRandomBinomial() * mWanderRate);

	mTargetOrientation = mWanderOrientation + (pOwner->getFacing() - (PIE / 2));

	//character orientation
	Vector2D currentDirection = to2DVector(pOwner->getFacing() - (PIE / 2));

	//target orientation
	Vector2D targetDirection = to2DVector(mTargetOrientation);

	mTargetLoc = pOwner->getPositionComponent()->getPosition() + currentDirection * wanderOffset;
	mTargetLoc += targetDirection * wanderRadius;
	mFaceSteering.setTargetLoc(mTargetLoc);

	Steering* mSteer = mFaceSteering.getSteering();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	/*
	if (mSteer != NULL)
	{*/
	data.rotAcc = mSteer->getData().rotAcc;
	data.rotVel = mSteer->getData().rotVel;
	/*	}*/
	data.acc = currentDirection * pOwner->getMaxAcc();

	this->mData = data;
	return this;
}